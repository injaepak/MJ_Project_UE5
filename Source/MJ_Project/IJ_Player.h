// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "IJ_Player.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FPlayerInputDelegate, class UInputComponent*);


UCLASS()
class MJ_PROJECT_API AIJ_Player : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

public:
	// Sets default values for this character's properties
	AIJ_Player();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlayerInfo)
		float CurrentHp = 500.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlayerInfo)
		float MaxHp = 500.f;

	void SetDamage(float damage)
	{
		CurrentHp -= damage;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		class UIJ_PlayerMovement* playerMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		class UIJ_PlayerFSM* playerFSM;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		class UIJ_DamageManager* damageManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		class AIJ_ActionCameraManager* actionCam;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AndroidBot)
		class USceneComponent* leftAndroidBotPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AndroidBot)
		class USceneComponent* rightAndroidBotPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AndroidBot)
		class USceneComponent* androidBotRootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AndroidBot)
		class USkeletalMeshComponent* androidBot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		class UBoxComponent* interactionZone;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* cameraCollision_L;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* cameraCollision_M;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* cameraCollision_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		class UChildActorComponent* cameraChild_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		class UChildActorComponent* cameraChild_M;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		class UChildActorComponent* cameraChild_R;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		bool bIsBaseAttackMontagePlaying = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		bool bIsDashAttackMontagePlaying = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		bool bIsBaseAttackPlaying = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		bool bIsDashPlaying = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		bool bIsEvasion = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		bool bIsTakeDamage = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		float canEvasionTime = 0.15f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		float currentEvasionTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		float damageAmountValue = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AndroidBot)
		float runningTime = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AndroidBot)
		float deltaHeight = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AndroidBot)
		bool bIsAndroidLeft = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AndroidBot)
		bool bIsAndroidAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AndroidBot)
		bool bIsCameraBlending = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AndroidBot)
		FVector androidSinLoc_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AndroidBot)
		FVector androidSinLoc_R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* baseAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		UAnimMontage* dashAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* enemy;

	FVector targetKnockbackLoc;

	// 사용자 입력처리 델리게이트
	FPlayerInputDelegate OnInputDelegate;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	void AndroidBot();

	void Evasion();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsSideView = false;

	UFUNCTION()
		void OnCollisionEnter(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndexs);
};
