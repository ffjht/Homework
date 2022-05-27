// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NonePlayableCharacter.h"
#include "..\Message\ContentsEnum.h"
#include "../Message/ServerToClient.h"
#include "../Message/ServerAndClient.h"
#include "../Message/ClientToServer.h"

#include "MyChracterWidget.h"
#include "Components/WidgetComponent.h"

#include "ClientMonster.generated.h"

DECLARE_DELEGATE(FOnHpChanged);

struct MonsterInfo
{
	// 가까이오면 바라만 보는 거리
	float LookRange;

	// 더 접근하면 쫒아오기 시작하는 거리.
	float TraceRange;

	// 공격이 가능한 거리.
	float AttRange;

	float Att;
	float Hp;
	float Speed;
};


/**
 * 
 */
UCLASS()
class UNREALCLIENT_API AClientMonster : public ANonePlayableCharacter
{
	GENERATED_BODY()


public:
	AClientMonster();
	virtual void PreInitializeComponents() override;
	virtual void ObjectInit() override;

protected:
	virtual void BeginPlay() override;
	
	void Tick(float DeltaTime);
	virtual void Destroyed() override;
	//////////////////////////////////////// ClientCode

private:
	EMonsterState State_;

	AActor* Target;

	MonsterInfo Info;
	float Ratio;
	int Hp = 1000;
	int MaxHp = 1000;

	// std::list<FMonsterUpdateData> UpdateList;
	TArray<FMonsterUpdateData> UpDataData;
	FMonsterUpdateData CurrentUpdateData;

	void Att(float _DelataTime);
	void Idle(float _DelataTime);
	void Trace(float _DelataTime);

	void AnimationStart(ClientAnimationType _Value);
	void AnimationEnd(ClientAnimationType _Value);


/////////////////////////////////////////////////////////////////////////////////////////// 클라이언트 함수
public:
	void ClientUpdate(float _DelataTime);
	void SetClientDamage(float Damage);
	void ClientObjectInit();

	int GetHp();
	int GetMaxHp();

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HpBar;
	//class UMyChracterWidget* HpWidget;		
	FOnHpChanged OnHpChanged;

};

