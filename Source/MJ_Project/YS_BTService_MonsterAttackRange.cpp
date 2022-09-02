// Fill out your copyright notice in the Description page of Project Settings.

#include "YS_BTService_MonsterAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "YS_MonsterAIController.h"
#include "YS_MonsterBase.h"

UYS_BTService_MonsterAttackRange::UYS_BTService_MonsterAttackRange()
{
	NodeName = TEXT("Monster Attack Range");
}


void UYS_BTService_MonsterAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AYS_MonsterAIController* const cont = Cast<AYS_MonsterAIController>(OwnerComp.GetAIOwner());
	AYS_MonsterBase* const monster = Cast<AYS_MonsterBase>(cont->GetPawn());

	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);


	DistanceToPlayer = monster->GetDistanceTo(player);
	//UE_LOG(LogTemp, Log, TEXT("%f"), DistanceToPlayer);


	//블랙보드 값 넘겨주는 부분
	cont->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("bInAttackRange")), DistanceToPlayer <= attack_Range);
	//bool debug = cont->GetBlackboardComponent()->GetValueAsBool(FName(TEXT("bInAttackRange")));
	//UE_LOG(LogTemp, Log, TEXT(" %s"), debug ? TEXT("true") : TEXT("false"));

}
