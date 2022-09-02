// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "YS_BTService_FindPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class MJ_PROJECT_API UYS_BTService_FindPlayerLocation : public UBTService
{
	GENERATED_BODY()
public:

	UYS_BTService_FindPlayerLocation();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;
};
