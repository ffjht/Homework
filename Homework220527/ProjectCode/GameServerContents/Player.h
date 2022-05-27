#pragma once
#include <GameServerCore\GameServerActor.h>
#include "ServerAndClient.h"
#include "ContentsEnum.h"

// ���� :
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

	//���߿� ���� ��ƾ� ������.
	float AttackAnimationTime = 0.6f;	//0.05�� ���� ������ �؇J��.
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
	//�׳� �޽����� ������ �� �ǳ�?
	//�޽����� ����� ������ ���� �� �ֱ�� ������..�׷��� Serailize ����.
	//�׳� int�� ����?

	void Update(float _Time) override;

	void SectionInitialize() override;
	void TCPSessionInitialize() override;
	void UDPSessionInitialize() override;

	bool InsertSection() override;

	//void CreateAttackCollision();
	// ��ü�μ� �����Ǵ� ��
	void DeathEvent() override;

	// �̰� ���� ���ǿ����� �����Ǳ� �����̰�
	// ������ �޸𸮰� �����Ǳ� ������ ȣ��Ǵ°�
	void DisConnect();

	void UpdateExp();
	void UpdateLevel();
};

