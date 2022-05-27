#include "PreCompile.h"
#include "Monster.h"
#include "Player.h"
#include "GameServerCore\GameServerSection.h"
#include <GameServerBase\GameServerMath.h>
#include "ContentsSystemEnum.h"

Monster::Monster() 
	: Target(nullptr)
	, UpdateTime(0.0f)
	, Hp_(1000)
	, Exp(100)
	, AttackPower_(100)
{
}

Monster::~Monster() 
{
}

void Monster::StateUpdate(float _DeltaTime)
{
	EMonsterState State = static_cast<EMonsterState>(Message_.Data.State);

	switch (State)
	{
	case EMonsterState::NONE:
		break;
	case EMonsterState::MState_Idle:
		IdleUpdate(_DeltaTime);
		break;
	case EMonsterState::MState_Trace:
		TraceUpdate(_DeltaTime);
		break;
	case EMonsterState::MState_Att:
		AttUpdate(_DeltaTime);
		break;
	case EMonsterState::MAX:
		break;
	default:
		break;
	}
}


void Monster::Update(float _Time)
{
	// ����ü�� ���⼭ ������ �־���Ҽ��� �ְ�.
	
	UpdateTime += _Time;


	if (nullptr != Target && false == Target->GetSectionCompare(GetSection()))
	{
		Target = nullptr;
		ChangeState(EMonsterState::MState_Idle);
		UpdateTime = 0.0f;
		return;
	}

	if (Message_.Data.State == static_cast<int>(EMonsterState::MState_Att))
	{
		AttUpdate(_Time);
		UpdateTime = 0.0f;
		return;
	}

	if (false == IsFrame(20))
	{
		return;
	}

	StateUpdate(UpdateTime);


	// ������ �ϰ�

	BroadcastingMonsterUpdateMessage();
	UpdateTime = 0.0f;
}

GameServerSerializer& Monster::GetSerializeMonsterUpdateMessage()
{
	Message_.Data.Dir = GetDir();
	Message_.Data.Pos = GetPos();
	Message_.Data.ObjectIndex = GetIndex();
	Message_.Data.Hp = Hp_;

	Serializer_.Reset();

	Message_.Data.DeltaTime = GetAccTime();

	Message_.Serialize(Serializer_);

	return Serializer_;
}

void Monster::BroadcastingMonsterUpdateMessage()
{
	// �ٸ��� �� �� �Ͼ������ ��� ���µ�
	// ������Ŷ ��ŭ�� tcp�� ������.

	
	if (true)
	{
		GetSection()->UDPBroadcasting(GetSerializeMonsterUpdateMessage().GetData());
	}

}

void Monster::SectionInitialize()
{

}
void Monster::TCPSessionInitialize()
{

}
void Monster::UDPSessionInitialize() 
{

}

bool Monster::InsertSection() 
{
	GameServerRandom Random;

	SetPos({ Random.Randomfloat(-900, 900), Random.Randomfloat(-900, 900), -200 });

	PlayerSensorCollision = GetSection()->CreateCollision(ECollisionGroup::MONSTERSENSOR, this);
	PlayerSensorCollision->SetScale({ 500.0f, 500.0f });

	HitCollision = GetSection()->CreateCollision(ECollisionGroup::MONSTER, this);
	HitCollision->SetScale({150.0f, 150.0f});
	ChangeState(EMonsterState::MState_Idle);

	return true;
}

// ��ü�μ� �����Ǵ� ��
void Monster::DeathEvent() 
{
	{
		//const std::list<std::shared_ptr<GameServerActor>>& AllOtherPlayer = GetSection()->GetPlayableActor();

		ObjectDestroyMessage Message;
		Message.ObjectID = GetIndex();

		GameServerSerializer Sr;
		Message.Serialize(Sr);

		int a = Hp_;
		GetSection()->TCPBroadcasting(Sr.GetData(), GetIndex());
	}
	
	HitCollision->Off();
	PlayerSensorCollision->Off();

	//HitCollision->Death();
	//HitCollision = nullptr;
	//PlayerSensorCollision->Death();
	//PlayerSensorCollision = nullptr;
}

void Monster::ChangeState(EMonsterState _State)
{
	switch (_State)
	{
	case EMonsterState::NONE:
		break;
	case EMonsterState::MState_Idle:
		IdleStart();
		break;
	case EMonsterState::MState_Trace:
		TraceStart();
		break;
	case EMonsterState::MState_Att:
		AttStart();
		break;
	case EMonsterState::MAX:
		break;
	default:
		break;
	}

	Message_.Data.State = static_cast<int>(_State);
}

void Monster::IdleStart() 
{
}


void Monster::TraceStart()
{

}

void Monster::AttStart() 
{
	AttTime = 1.0f;
}

void Monster::IdleUpdate(float _DeltaTime)
{
	static std::vector<GameServerCollision*> Result;
	// �浹 �߾� ���߾�?

	if (true == PlayerSensorCollision->CollisionCheckResult(CollisionCheckType::SPHERE, ECollisionGroup::PLAYER, CollisionCheckType::SPHERE, Result))
	{
		Target = Result[0]->GetOwnerActor();

		int a = 0;

		ChangeState(EMonsterState::MState_Trace);
		return;
	}

	Result.clear();

}
void Monster::TraceUpdate(float _DeltaTime)
{
	if (nullptr == Target)
	{
		ChangeState(EMonsterState::MState_Idle);
		return;
	}

	if ( 150.0f >= (Target->GetPos() - GetPos()).Length3DReturn())
	{
		ChangeState(EMonsterState::MState_Att);
		// BroadcastingMonsterUpdateMessage();
		return;
	}
	else 
	{
		SetDir((Target->GetPos() - GetPos()).Normalize3DReturn());
		//    1, 0               0.0
		Move((Target->GetPos() - GetPos()).Normalize3DReturn() * 400.0f * _DeltaTime);
	}

	if (false == Target->GetSectionCompare(GetSection()))
	{
		Target = nullptr;
		ChangeState(EMonsterState::MState_Idle);
		return;
	}

	if (false == Target->IsUpdate()
		|| true == Target->IsSectionMove()
		|| Target->IsDeath())
	{
		Target = nullptr;
		ChangeState(EMonsterState::MState_Idle);
		return;
	}
}


void Monster::AttUpdate(float _DeltaTime)
{
	AttTime -= _DeltaTime;
	if (0.0f >= AttTime)
	{
		AttTime = 0.0f;
		ChangeState(EMonsterState::MState_Idle);
		return;
	}

}

void Monster::SetHp(int _Hp)
{
	if (_Hp < 0)
	{
		Hp_ = 0;
	}

	Hp_ = _Hp;
}

int Monster::GetHp()
{
	return Hp_;
}

int Monster::GetExp()
{
	return Exp;
}