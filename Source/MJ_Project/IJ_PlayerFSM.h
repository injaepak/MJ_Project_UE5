// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IJ_PlayerFSM.generated.h"

// 플레이어 상태 정의
UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Idle,
	Move,
	Run,
	Dash,
	DashAttack,
	Attack,
	Defence,
	Damaged,
	Die,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MJ_PROJECT_API UIJ_PlayerFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UIJ_PlayerFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FSM)
	EPlayerState m_state;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
	class UIJ_Player_AnimInstance* anim;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		class UIJ_PlayerMovement* playerMovement;*/

	UPROPERTY()
		class AIJ_Player* me;

private:

	void IdleState();
	void MoveState();
	void RunState();
	void DashState();
	void DashAttackState();
	void AttackState();
	void DefenceState();
	void DamagedState();
	void DieState();
};
