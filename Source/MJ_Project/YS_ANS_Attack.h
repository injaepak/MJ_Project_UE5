// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "YS_ANS_Attack.generated.h"

/**
 * 
 */
UCLASS()
class MJ_PROJECT_API UYS_ANS_Attack : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	

	

private :
	UPROPERTY(EditAnywhere)
		TEnumAsByte<EDrawDebugTrace::Type> DebugType;

	UPROPERTY(EditAnywhere)
		FName SocketName;

	UPROPERTY(EditAnywhere)
		float AttackDistance = 200;

	UPROPERTY(EditAnywhere)
		float AttackRadius = 10;

	UPROPERTY(EditAnywhere)
		float AttackDamage = 0;

	UPROPERTY(EditAnywhere)
		bool ClinchStart = false;

	UPROPERTY()
		bool bCanAttack = true;

	

	UPROPERTY(EditAnywhere)
		FVector RotatorVector = {};


	UPROPERTY(EditAnywhere)
		TArray<APawn*> TargetPawn;

	UPROPERTY(EditAnywhere)
		TArray<APawn*> DamagedPawn;

	


};
