// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "YS_MonsterBase.h"
#include "YS_BTTask_MonsterAttack.generated.h"

/**
 * 
 */
UCLASS()
class MJ_PROJECT_API UYS_BTTask_MonsterAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UYS_BTTask_MonsterAttack(FObjectInitializer const& object_intializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)override;

};
