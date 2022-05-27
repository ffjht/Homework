#pragma once
#include <GameServerCore\GameServerActor.h>
#include "ServerAndClient.h"
#include "ContentsEnum.h"

// 설명 :
class ContentsUserData;
class Player : public GameServerActor
{
public:
	// constrcuter destructer
	Player();
	~Player();

	// delete Function
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

public:
	void MessageCheck();

	void ClientToReadyMessageProcess(std::shared_ptr<class ClientToReadyMessage> _Message);
	void PlayerUpdateMessageProcess(std::shared_ptr<class PlayerUpdateMessage> _Message);
	void LevelMoveReplyMessageProcess(std::shared_ptr<class LevelMoveReplyMessage> _Message);
	void SetReceptionDelayTime();
	bool SetExp(int _Exp);
	
	int GetExp();
	int GetLevel();

protected:
	PlayerUpdateMessage& GetPlayerUpdateMessage();
	GameServerSerializer& GetSerializePlayerUpdateMessage();

	void SelfUpdateMessage();
	void BroadcastingPlayerUpdateMessage();
	void TCPBroadcastingPlayerUpdateMessage();

private:
	class Portal* PortalPtr;

	// std::shared_ptr<GameServerCollision> HitCollision;
	GameServerCollision* HitCollision;
	GameServerCollision* AttackCollision;

	PlayerUpdateMessage Message_;
	GameServerSerializer Serializer_;
	
	float AttackMessageArriveTime;

	//나중에 따로 모아야 될지도.
	float AttackAnimationTime = 0.6f;	//0.05초 정도 빠르게 해놧다.
	float ReceptionDelayTime;
	float TransmissionDelayTime = 0.1f;
	
	float AttackRange = 150.0f;

	std::shared_ptr<ContentsUserData> UserData;
	
	bool UDPReady_;
	bool InAttack;

	int Hp;
	int Exp_;
	int Level_;
	int AttackPower;
	int State;
	//그냥 메시지에 넣으면 안 되나?
	//메시지에 아토믹 변수를 넣을 수 있기는 하지만..그러면 Serailize 문제.
	//그냥 int로 간주?

	void Update(float _Time) override;

	void SectionInitialize() override;
	void TCPSessionInitialize() override;
	void UDPSessionInitialize() override;

	bool InsertSection() override;

	//void CreateAttackCollision();
	// 객체로서 삭제되는 것
	void DeathEvent() override;

	// 이건 지금 섹션에서도 삭제되기 직전이고
	// 완전히 메모리가 정리되기 직전에 호출되는것
	void DisConnect();

	void UpdateExp();
	void UpdateLevel();
};

