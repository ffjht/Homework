// Fill out your copyright notice in the Description page of Project Settings.


#include "MyChracterWidget.h"
#include "ClientMonster.h"

void UMyChracterWidget::BindHp(class AClientMonster* Monster)
{
	MonsterPtr = Monster;
	Monster->OnHpChanged.BindUFunction(this, FName("UpdateHp"));
}

void UMyChracterWidget::UpdateHp()
{
	float HpRatio = static_cast<float>(MonsterPtr->GetHp()) / static_cast<float>(MonsterPtr->GetMaxHp());
	PB_HpBar->SetPercent(HpRatio);
}