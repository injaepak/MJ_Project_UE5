// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IJ_SideCamera.generated.h"

UCLASS()
class MJ_PROJECT_API AIJ_SideCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIJ_SideCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		class USceneComponent* rootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		class UCameraComponent* sideCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		class AIJ_Player* player;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		FVector myLoc = FVector(0.f, 0.f, 0.f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		float myLocX = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		float targetLocX = 0.f;
};
