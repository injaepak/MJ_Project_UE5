// Fill out your copyright notice in the Description page of Project Settings.

#include "YS_BTService_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "IJ_Player.h"
#include "Kismet/GameplayStatics.h"

UYS_BTService_FindPlayerLocation::UYS_BTService_FindPlayerLocation()
{

}

void UYS_BTService_FindPlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (PlayerCharacter)
	{
		if (OwnerComp.GetBlackboardComponent())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("PlayerLocation")), 
				FVector(PlayerCharacter->GetActorLocation()));
		}
		return;
	}
}
