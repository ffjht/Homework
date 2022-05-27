#pragma once
#include <GameServerCore\GameServerActor.h>
#include "ServerToClient.h"
#include "Monster.h"



// 설명 :
class Monster : public GameServerActor
{
public:
	// constrcuter destructer
	Monster();
	~Monster();

	// delete Function
	Monster(const Monster& _Other) = delete;
	Monster(Monster&& _Other) noexcept = delete;
	Monster& operator=(const Monster& _Other) = delete;
	Monster& operator=(Monster&& _Other) noexcept = delete;

protected:
	MonsterUpdateMessage Message_;
	GameServerSerializer Serializer_;

	GameServerActor* Target;

	float UpdateTime;
	float AttTime;

	int Hp_;
	//사실 몬스터의 Exp는 따로 몬스터 정보를 모아놓은 헤더를 만드는 것이 좋을 듯.
	int Exp;
	int AttackPower_;
	uint64_t KillPlayer_;

	// void SelfUpdateMessage();
	GameServerSerializer& GetSerializeMonsterUpdateMessage();
	void BroadcastingMonsterUpdateMessage();

	GameServerCollision* PlayerSensorCollision;
	GameServerCollision* HitCollision;

private:
	void Update(float _Time) override;

	void SectionInitialize() override;
	void TCPSessionInitialize() override;
	void UDPSessionInitialize() override;

	bool InsertSection() override;

	// 객체로서 삭제되는 것
	void DeathEvent() override;

public:
	void ChangeState(EMonsterState _State);

	void StateUpdate(float _DeltaTime);

	void IdleStart();
	void TraceStart();
	void AttStart();

	void IdleUpdate(float _DeltaTime);
	void TraceUpdate(float _DeltaTime);
	void AttUpdate(float _DeltaTime);
	void SetHp(int _Hp);
	
	int GetHp();
	int GetExp();
};

