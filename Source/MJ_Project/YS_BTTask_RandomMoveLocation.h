// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "YS_BTTask_RandomMoveLocation.generated.h"

/**
 * 
 */
UCLASS()
class MJ_PROJECT_API UYS_BTTask_RandomMoveLocation : public UBTTaskNode
{
	GENERATED_BODY()
		
public:
	UYS_BTTask_RandomMoveLocation();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);


	UPROPERTY(EditAnywhere)
		float random_radius = 500.f;
	
};
