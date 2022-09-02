// Fill out your copyright notice in the Description page of Project Settings.

#include "YS_BTTask_RandomMoveLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "YS_MonsterAIController.h"
#include "NavigationSystem.h"

UYS_BTTask_RandomMoveLocation::UYS_BTTask_RandomMoveLocation()
{
	NodeName = TEXT("FindRandomLocation");
}

EBTNodeResult::Type UYS_BTTask_RandomMoveLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//OwnerPawn���� ����
	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return EBTNodeResult::Failed;

	//Nav���� ����
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
		return EBTNodeResult::Failed;
	

	//RandomNavRadius�Ű�����
	auto PawnLocation = CurrentPawn->GetActorLocation();
	FNavLocation RandomLocation;
	
	//���� �Լ�����
	if (NavSystem->GetRandomPointInNavigableRadius(PawnLocation, 500.f, RandomLocation))
	{
		//UE_LOG(LogTemp, Log, TEXT("%s"), *RandomLocation.Location.ToString());

		//BlackBoard�� �� �Ѱ��ֱ�
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("RandomLocation")),RandomLocation.Location);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
