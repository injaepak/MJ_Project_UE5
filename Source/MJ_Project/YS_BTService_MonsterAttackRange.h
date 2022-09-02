// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "YS_BTService_MonsterAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class MJ_PROJECT_API UYS_BTService_MonsterAttackRange : public UBTService
{
	GENERATED_BODY()

public:
	UYS_BTService_MonsterAttackRange();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		float attack_Range = 100.0f;

	UPROPERTY(BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
		float DistanceToPlayer = 0.f;
};
