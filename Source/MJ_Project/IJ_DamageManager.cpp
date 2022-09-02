// Fill out your copyright notice in the Description page of Project Settings.
#include "IJ_DamageManager.h"
#include "IJ_Player.h"
#include "IJ_PlayerFSM.h"
#include "IJ_Player_AnimInstance.h"
#include "GameFramework/Pawn.h"


// Sets default values for this component's properties
UIJ_DamageManager::UIJ_DamageManager() : hp(100), isDead(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UIJ_DamageManager::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<AIJ_Player>(GetOwner());

}


// Called every frame
void UIJ_DamageManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	currentTime += DeltaTime;

	// ¸¸¾à Defence
	if (bDefenceSlowOn == true)
	{
		slowTime += DeltaTime;

		playerPos = FMath::Lerp(playerPos, knockbackPos, GetWorld()->DeltaTimeSeconds);
		me->SetActorLocation(playerPos);


		if (slowTime >= maxSlowTime)
		{
			GetWorld()->GetWorldSettings()->SetTimeDilation(1.f);
			slowTime = 0.f;
			bDefenceSlowOn = false;
		}
	}
}


void UIJ_DamageManager::SetHP(float Damage)
{
	if (me->playerFSM->m_state != EPlayerState::Defence)
	{
		hp -= Damage;
		me->playerFSM->m_state = EPlayerState::Damaged;
	}

	else if (me->playerFSM->m_state == EPlayerState::Defence)
	{
		bDefenceSlowOn = true;

		myPos = me->GetActorLocation();
		playerPos = me->GetActorLocation();
		knockbackPos = myPos + me->targetKnockbackLoc * knockbackValue;

		GetWorld()->GetWorldSettings()->SetTimeDilation(0.25f);
	}
	if (hp <= 0)
		Die();

}

void UIJ_DamageManager::Die()
{
	isDead = true;
}


