// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IJ_ActionCameraManager.generated.h"

UCLASS()
class MJ_PROJECT_API AIJ_ActionCameraManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIJ_ActionCameraManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
		class USceneComponent* rootComp;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		class AIJ_ActionTopCamera* topCam;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		class AIJ_ActionBackCamera* backCam;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		class AIJ_ActionLeftCamera* leftCam;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		class AIJ_ActionRightCamera* rightCam;*/
};
