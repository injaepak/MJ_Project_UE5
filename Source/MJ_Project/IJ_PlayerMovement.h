// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IJ_PlayerMovement.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MJ_PROJECT_API UIJ_PlayerMovement : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UIJ_PlayerMovement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;



public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

	UPROPERTY()
		class AIJ_Player* me;

	UPROPERTY(EditDefaultsOnly, Category = "AvoidFactory")
		TSubclassOf<class AIJ_AvoidActor> avoidActorFactory;

public:

	void MoveForward(float Value);

	void MoveRight(float Value);

	void Jump();

	void StopJumping();

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void Turn(float Rate);

	void LookUp(float Rate);

	void Dash(FKey key);

	void Attack();

	void DefenceOn();

	void StopDefence();

	void Evasion(FKey key);

	void AndroidBotFire(float Value);

	void AndroidBotAttackOn();

	void AndroidBotAttackOff();


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		float dashDistance = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		float evasionDistance = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		float walkSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		float dashCurrentTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		float dashDelayTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		float fireCurrentTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		float fireDelayTime = 0.1f;


public:

	UPROPERTY(EditDefaultsOnly, Category = Fire)
		TSubclassOf<class AIJ_AndroidBotBullet> bulletFactory;

	UPROPERTY(EditAnywhere, Category = Shake)
		TSubclassOf<class UCameraShakeBase> bulletCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = Fire)
		class UNiagaraSystem* fireStartFX;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		bool bIsKillMontage = false;
};
