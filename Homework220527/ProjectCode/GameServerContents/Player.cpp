#include "PreCompile.h"
#include "Player.h"
#include "Portal.h"
#include "Monster.h"
#include "ClientToServer.h"
#include "ServerToClient.h"
#include "GameServerCore\GameServerCore.h"
#include "GameServerBase\GameServerThread.h"

#include <GameServerNet\TCPSession.h>
#include "ContentsSystemEnum.h"
#include "ContentsUserData.h"
#include "CharacterTable.h"
#include "GameServerBase\GameServerDebug.h"
#include <GameServerCore\GameServerSection.h>
#include <GameServerCore\DBQueue.h>
#include <GameServerNet\RedisConnecter.h>

#include <chrono>

Player::Player()
	: UDPReady_(false)
	, InAttack(false)
	, PortalPtr(nullptr)
	, HitCollision(nullptr)
	, AttackCollision(nullptr)
	, AttackPower(100)
	, State(static_cast<int>(EPlayerState::PState_Idle))
{
}

Player::~Player() 
{

	if (nullptr != HitCollision)
	{
		HitCollision->Death();
		HitCollision = nullptr;
	}

	if (nullptr != AttackCollision)
	{
		AttackCollision->Death();
		AttackCollision = nullptr;
	}
}

void Player::PlayerUpdateMessageProcess(std::shared_ptr<class PlayerUpdateMessage> _Message)
{
	// �̰� ���� ó������ ���ƿ� �޼������.
	// ������ ��� �ְ� �Ǿ���.

	//ó�� PlayerUpdateMessage�� ���ƿ��� ��, UDPReady_�� ���ִ� ���.
	if (false == UDPReady_)
	{
		UDPReady_ = true;
		UDPReadyOKMessage Msg;
		Msg.Code = EGameServerCode::OK;
		Msg.ObjectIndex = GetIndex();
		GameServerSerializer Sr;
		Msg.Serialize(Sr);

		// "SELECT Idx, NickName, UserIndex, ATT, HP, LastSectionid, LastSectionPosx, LastSectionPosy, LastSectionPosz FROM userver2.characterinfo WHERE NickName = ? LIMIT 1"
		GetTCPSession()->Send(Sr.GetData());

		// GetTCPSession()->Send("SELECT Idx, NickName, UserIndex, ATT, HP, LastSectionid, LastSectionPosx, LastSectionPosy, LastSectionPosz FROM userver2.characterinfo WHERE NickName = ? LIMIT 1");
		return;
	}

	//PlayerUpdateMessage�� ���ƿ��� �� ������ Player�� �����ϰų�
	//�ٸ� Player�鿡�� ��ε� ĳ���� ���ִ� ����.(����� ��ġ ������ ������.)

	Message_.Data = _Message->Data;
	Message_.Time = _Message->Time;
	// (_Message->Data.Pos - _Message->Data.Pos).Length2D();

	
	SetPos(_Message->Data.Pos);
	SetDir(_Message->Data.Dir);
	State = _Message->Data.State;

	BroadcastingPlayerUpdateMessage();
}

// Ŭ���̾�Ʈ���׼� ������ �Դ�.
void Player::ClientToReadyMessageProcess(std::shared_ptr<ClientToReadyMessage> _Message)
{
	// �ʹ� �߿��ؼ� tcp�� ������.
	//// ���� �÷��̾� ��Ŷ ����
	{
		const std::list<std::shared_ptr<GameServerActor>>& AllOtherPlayer = GetSection()->GetPlayableActor();

		PlayersUpdateMessage Message;
		GameServerSerializer Sr;

		for (auto& OtherActor : AllOtherPlayer)
		{
			// ���� ����
			if (GetIndex() == OtherActor->GetIndex())
			{
				continue;
			}

			std::shared_ptr<Player> OtherPlayer = std::dynamic_pointer_cast<Player>(OtherActor);

			OtherPlayer->GetSerializePlayerUpdateMessage();
			Message.Datas.push_back(OtherPlayer->Message_.Data);
		}

		// �ֺ� ĳ���͵� ���δ� ������ ���.
		Message.Serialize(Sr);
		GetTCPSession()->Send(Sr.GetData());
		BroadcastingPlayerUpdateMessage();
	}

	SelfUpdateMessage();
}

void Player::LevelMoveReplyMessageProcess(std::shared_ptr<class LevelMoveReplyMessage> _Message)
{
	if (nullptr == PortalPtr)
	{
		GameServerDebug::AssertDebugMsg("��Ż�� �߸��� ��ü�� �� �־����ϴ�.");
		return;
	}

	LevelMoveMessage Msg;
	Msg.ObjectIndex = GetIndex();
	Msg.SectionIndex = GetSectionIndex();
	Msg.ThreadIndex = GetThreadIndex();
	Msg.MoveLevel = PortalPtr->LinkSection->GetNameCopy();
	GameServerSerializer Sr;
	Msg.Serialize(Sr);
	GetSection()->TCPBroadcasting(Sr.GetData(), GetIndex());

	GetSection()->SectionMove(DynamicCast<GameServerActor>(), PortalPtr->LinkSection);
	PortalPtr = nullptr;
}

void Player::SetReceptionDelayTime()
{
	//typedef duration<int, ratio_multiply<hours::period, ratio<24>>::type> days;
	//std::chrono::system_clock::time_point TPServerCurTime = system_clock::now();
	//std::chrono::system_clock::duration tp = TPServerCurTime.time_since_epoch();

	//std::time_t newt = std::chrono::system_clock::to_time_t(TPServerCurTime);
	/*struct tm gmTM;

	std::time_t ServerCurTime = time(NULL);
	gmtime_s(&gmTM, &ServerCurTime);*/

	//ms���� ����.
	/*days d = duration_cast<days>(tp);
	tp -= d;
	hours h = duration_cast<hours>(tp);
	tp -= h;
	minutes m = duration_cast<minutes>(tp);
	tp -= m;
	seconds s = duration_cast<seconds>(tp);
	tp -= s;
	milliseconds ms = duration_cast<milliseconds>(tp);*/

	std::tm timeinfo = std::tm();
	timeinfo.tm_year = Message_.Time.Year - 1900;   
	timeinfo.tm_mon = Message_.Time.Month - 1;
	timeinfo.tm_mday = Message_.Time.Date;    
	timeinfo.tm_hour = Message_.Time.Hour;
	timeinfo.tm_min = Message_.Time.Minute;
	timeinfo.tm_sec = Message_.Time.Second;
	
	std::time_t TMRecvTime = _mkgmtime(&timeinfo);

	std::chrono::system_clock::time_point TPRecvTime = std::chrono::system_clock::from_time_t(TMRecvTime);
	std::chrono::system_clock::duration ns = std::chrono::system_clock::now() - TPRecvTime;

	typedef std::chrono::duration<int, std::nano> sec_type;
	sec_type nsecs = std::chrono::duration_cast<sec_type> (ns);

	ReceptionDelayTime = static_cast<float>(static_cast<int>(nsecs.count()) / 1'000'000'000 - Message_.Time.Millisecond / 1000);
}

bool Player::SetExp(int _Exp)
{
	//���Ѽ� �Ѵ� ��� ó��.
	if (_Exp / 100 > Level_)
	{
		Level_ = _Exp / 100;
		_Exp = _Exp % 100;
		return true;
	}

	Exp_ = _Exp;

	return false;
}

int Player::GetExp()
{
	return Exp_;
}

int Player::GetLevel()
{
	return Level_;
}

void Player::Update(float _DeltaTime)
{
	MessageCheck();


	if (0 == (static_cast<int>(GetAccTime()) % 10) )
	{
		// DBQueue::Queue(std::bind(_ChildFunction, std::dynamic_pointer_cast<ChildThreadHandler>(this->shared_from_this())));

		std::string Name = UserData->SelectData.NickName;

		DBQueue::Queue([=]{
			RedisConnecter* Connecter = GameServerThread::GetLocalData<RedisConnecter>(1);
			RedisCommend::ZADD MyAdd = RedisCommend::ZADD("UserRank", 10000, Name);
			MyAdd.Process(*Connecter);


		});
		// dbqueue
	}

	if (false == InAttack &&  State == static_cast<int>(EPlayerState::PState_Att))
	{
		InAttack = true;
		AttackMessageArriveTime = GetAccTime();
		SetReceptionDelayTime();
	}

	if (true == InAttack && AttackMessageArriveTime + AttackAnimationTime - ReceptionDelayTime - TransmissionDelayTime <= GetAccTime())
	{
		if (nullptr == AttackCollision)
		{
			return;
		}

		//AttackCollision�� ���� �ٸ� �ݸ��� �׷����� ���ϱ� ���� On, Off�� �ᵵ �ȴ�.
		AttackCollision->On();

		//AttackCollision��ġ ����.
		//Collision�� Actor�� �ƴ϶� Pivot�� ��ġ ����?
		//�Լ��� ���� ����°� ��������.
		//void GameServerCollision::CollisionDataUpdate()

		//SetDir((Target->GetPos() - GetPos()).Normalize3DReturn());

		//���� ��û�� 
		AttackCollision->SetPivot(GetDir() * AttackRange);
		
		//���� ��ġ GetDir() + 	
		static std::vector<GameServerCollision*> MonsterCollisionResult;
		
		if (true == AttackCollision->CollisionCheckResult(CollisionCheckType::SPHERE, ECollisionGroup::MONSTER, CollisionCheckType::SPHERE, MonsterCollisionResult))
		{
			//������ �浹�� ����Ų ���Ͱ� �ϳ���� ����.....
			Monster* TargetMonster = MonsterCollisionResult[0]->GetOwnerActorConvert<Monster>();

			int TargetMonsterCurHp = TargetMonster->GetHp();
			if (TargetMonsterCurHp > 0)
			{
				TargetMonster->SetHp(TargetMonsterCurHp - AttackPower);
				if (TargetMonster->GetHp() == 0)
				{
					TargetMonster->Death();

					if (SetExp(Exp_ + TargetMonster->GetExp()))
					{
						UpdateLevel();			
					}
					UpdateExp();				
				}
			}
			MonsterCollisionResult.clear();
		}
		InAttack = false;
		State = static_cast<int>(EPlayerState::PState_Idle);
		AttackCollision->Off();
	}
	
	//��Ż �浹 üũ
	if (IsFrame(10))
	{
		if (nullptr == HitCollision)
		{
			return;
		}

		static std::vector<GameServerCollision*> Result;
		// �浹 �߾� ���߾�?

		if (true == HitCollision->CollisionCheckResult(CollisionCheckType::SPHERE, ECollisionGroup::POTAL, CollisionCheckType::SPHERE, Result))
		{
			PortalPtr = Result[0]->GetOwnerActorConvert<Portal>();

			if (nullptr == PortalPtr)
			{
				GameServerDebug::AssertDebugMsg("��Ż�� �߸��� ��ü�� �� �־����ϴ�.");
				return;
			}

			// ���ƿ��� ��Ŷ�鵵 �� �����ؾ���.


			// ������ �ִ� �ٸ� ���͵鿡�� �������ϴ� �޼����� �˴ϴ�.
			LevelMoveMessage Msg;
			Msg.ObjectIndex = GetIndex();
			Msg.SectionIndex = GetSectionIndex();
			Msg.ThreadIndex = GetThreadIndex();
			Msg.MoveLevel = PortalPtr->LinkSection->GetNameCopy();
			GameServerSerializer Sr;
			Msg.Serialize(Sr);
			// GetSection()->TCPBroadcasting(Sr.GetData(), GetIndex());

			// Off();

			GetTCPSession()->Send(Sr.GetData());


			//HitCollision->Off();
			//AttackCollision->Off();

			HitCollision->Death();
			HitCollision = nullptr;

			AttackCollision->Death();
			AttackCollision = nullptr;
		}

		Result.clear();

	}


	// ���� �����
}

PlayerUpdateMessage& Player::GetPlayerUpdateMessage()
{
	Message_.Data.Dir = GetDir();
	Message_.Data.Pos = GetPos();

	return Message_;
}

GameServerSerializer& Player::GetSerializePlayerUpdateMessage()
{
	Message_.Data.Dir = GetDir();
	Message_.Data.Pos = GetPos();
	Message_.Data.Exp = GetExp();
	Message_.Data.Level = GetLevel();

	Serializer_.Reset();

	Message_.Serialize(Serializer_);

	return Serializer_;
}

void Player::BroadcastingPlayerUpdateMessage() 
{
	GetSection()->UDPBroadcasting(GetSerializePlayerUpdateMessage().GetData(), GetIndex());
}

void Player::TCPBroadcastingPlayerUpdateMessage()
{
	GetSection()->TCPBroadcasting(GetSerializePlayerUpdateMessage().GetData(), GetIndex());
}

void Player::SelfUpdateMessage()
{
	GetTCPSession()->Send(GetSerializePlayerUpdateMessage().GetData());
}

bool Player::InsertSection() 
{
	if (nullptr == UserData)
	{
		GameServerDebug::AssertDebugMsg("UserData Setting Is Nullptr");
		return false;
	}

	Message_.Data.ObjectIndex = GetIndex();
	Message_.Data.SectionIndex = GetSectionIndex();
	Message_.Data.ThreadIndex = GetThreadIndex();
	
	// SetPos({200, 0.0f, 0.0f});

	// ���� ���� ��Ŷ ����
	{
		SectionInsertResultMessage Msg;
		Msg.Code = EGameServerCode::OK;
		Msg.UDPPort = GetUDPPort();
		Msg.ObjectIndex = GetIndex();
		Msg.SectionIndex = GetSectionIndex();
		Msg.ThreadIndex = GetThreadIndex();
		Msg.MoveLevel = GetSection()->GetNameCopy();
		GameServerSerializer Sr;
		Msg.Serialize(Sr);
		GetTCPSession()->Send(Sr.GetData());
	}

	SetPos(FVector4::ZeroVector);

	// Ŭ���̾�Ʈ�� ���� �غ� �ƴ�.



	GameServerDebug::LogInfo("Select Result OK Send");

	return true;
}

//void Player::CreateAttackCollision()
//{
//	if (nullptr == AttackCollision)
//	{
//		AttackCollision = GetSection()->CreateCollision(ECollisionGroup::PLAYERATTACK, this);
//		AttackCollision->SetScale({ 100.0f, 100.0f, 100.0f });
//	}
//
//	GetPos();
//	UDPReady_ = false;
//}

void Player::SectionInitialize() 
{
	// GetSection();

	SetScale({ 50.0f, 50.0f, 50.0f});
	if (nullptr == HitCollision)
	{
		HitCollision = GetSection()->CreateCollision(ECollisionGroup::PLAYER, this);
		HitCollision->SetScale({ 50.0f, 50.0f, 50.0f });
	}

	if (nullptr == AttackCollision)
	{
		AttackCollision = GetSection()->CreateCollision(ECollisionGroup::PLAYER, this);
		AttackCollision->SetScale({ 50.0f, 50.0f, 50.0f });
	}

	UDPReady_ = false;
}

void Player::TCPSessionInitialize() 
{
	UserData = GetTCPSession()->GetLink<ContentsUserData>(EDataIndex::USERDATA);

	if (nullptr == UserData)
	{
		GameServerDebug::AssertDebugMsg("UserData Setting Is Nullptr");
		return;
	}
}

void Player::UDPSessionInitialize()
{

	int a = 0;
}

void Player::DeathEvent()
{
	// GetSession()->UnregisterSession();
	{
		const std::list<std::shared_ptr<GameServerActor>>& AllOtherPlayer = GetSection()->GetPlayableActor();

		ObjectDestroyMessage Message;
		Message.ObjectID = GetIndex();

		GameServerSerializer Sr;
		Message.Serialize(Sr);

		for (auto& OtherActor : AllOtherPlayer)
		{
			// ���� ����
			if (GetIndex() == OtherActor->GetIndex())
			{
				continue;
			}

			std::shared_ptr<Player> OtherPlayer = std::dynamic_pointer_cast<Player>(OtherActor);
			OtherPlayer->GetTCPSession()->Send(Sr.GetData());
		}
	}

	PlayerDestroyMessage Message;
	GameServerSerializer Sr;
	Message.Serialize(Sr);
	GetTCPSession()->Send(Sr.GetData());
}

void Player::DisConnect()
{
	//// ���� �÷��̾� ��Ŷ ����

}

void Player::UpdateExp()
{
	//��� TCP�� ������.
	SelfUpdateMessage();
}

void Player::UpdateLevel()
{
	//Message_.Data.Level = Level_;
	//GetSerializePlayerUpdateMessage()���� �����.

	//GetPlayerUpdateMessage();
	//GetSerializePlayerUpdateMessage();

	//�� Broadcastring�� UDP�� ������.
	//�׷��� ���� ������Ʈ�� TCP�� �´� �� ����.

	//

	SelfUpdateMessage();
	//�ᱹ���� PlayerUpdateMessage()�� 
	TCPBroadcastingPlayerUpdateMessage();
}