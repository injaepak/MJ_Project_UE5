// Fill out your copyright notice in the Description page of Project Settings.

#include "YS_BTTask_MonsterAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "YS_MonsterAIController.h"
#include "YS_MonsterBase.h"

UYS_BTTask_MonsterAttack::UYS_BTTask_MonsterAttack(FObjectInitializer const& object_intializer)
{
	NodeName = TEXT("BasicAttack");
}

EBTNodeResult::Type UYS_BTTask_MonsterAttack::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	AAIController* const cont = owner_comp.GetAIOwner();
	AYS_MonsterBase* const monster = Cast<AYS_MonsterBase>(cont->GetPawn());
	
	if (monster)
	{
		{
			monster->Attack(monster->GetBasicAttackMontage());
		}

		//UE_LOG(LogTemp, Log, TEXT("Attack"));
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

