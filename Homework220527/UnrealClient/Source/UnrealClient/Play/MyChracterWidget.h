// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"

#include "MyChracterWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCLIENT_API UMyChracterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindHp(class AClientMonster* Monster);
	UFUNCTION()
	void UpdateHp();

private:
	AClientMonster* MonsterPtr;

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* PB_HpBar;
};
