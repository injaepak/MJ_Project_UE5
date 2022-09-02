// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IJ_BlendTriggerVolume.generated.h"

UCLASS()
class MJ_PROJECT_API AIJ_BlendTriggerVolume : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AIJ_BlendTriggerVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* OverlapVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AIJ_TopCamera* topCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AIJ_SideCamera* sideCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AIJ_Player* player;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.0f))
		float CameraBlendTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CurrentCameraBlendTime_In = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float CurrentCameraBlendTime_Out = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIstriggerOn = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIstriggerOff = false;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);

	virtual void NotifyActorEndOverlap(AActor* OtherActor);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};