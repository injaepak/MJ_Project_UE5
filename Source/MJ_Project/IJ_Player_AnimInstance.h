// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "IJ_PlayerFSM.h"
#include "IJ_Player_AnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class MJ_PROJECT_API UIJ_Player_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		EPlayerState state;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* idleMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* dashForwardMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* dashBackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* dashLeftMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* dashRightMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* baseAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* dashAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* baseDamagedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* EvasionForwardMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* EvasionBackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* EvasionLeftMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* EvasionRightMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* killMontage;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* killMontage_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* killMontage_3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* killMontage_4;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* doubleJumpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* damaged_OffMontage;

public:

	void Idle();
	void DashForward();
	void DashBack();
	void DashLeft();
	void DashRight();
	void Damaged_Off();
	void Run();
	void BaseAttack();
	void DashAttack();
	void Damaged();
	void DoubleJump();

	void EvasionForward();
	void EvasionBack();
	void EvasionLeft();
	void EvasionRight();

	void kill();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		bool bIsDefenceOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		bool bIsInAir = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		bool bCanIdle = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		bool bCanAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		bool bCanDash = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		bool bCanDefence = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		bool bCanDameged = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		bool bCanJump = false;
};
