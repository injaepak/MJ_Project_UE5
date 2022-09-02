// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "YS_MonsterWidget.generated.h"

/**
 * 
 */
UCLASS()
class MJ_PROJECT_API UYS_MonsterWidget : public UUserWidget
{
	GENERATED_BODY()
public :
		void BindHp(class AYS_MonsterBase* Monster);

	void UpdateHp();

private:
	TWeakObjectPtr<class AYS_MonsterBase> CurrentMonster;
	

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* MonsterHp;
};
