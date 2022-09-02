// Fill out your copyright notice in the Description page of Project Settings.


#include "YS_MonsterWidget.h"
#include "Components/ProgressBar.h"
#include "YS_MonsterBase.h"

void UYS_MonsterWidget::BindHp(AYS_MonsterBase* Monster)
{
	CurrentMonster = Monster;
	Monster->OnHpChanged.AddUObject(this, &UYS_MonsterWidget::UpdateHp);
}

void UYS_MonsterWidget::UpdateHp()
{
	if (CurrentMonster.IsValid())
	MonsterHp->SetPercent(CurrentMonster->GetHpRatio());
}
