// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IJ_AvoidActor.generated.h"

UCLASS()
class MJ_PROJECT_API AIJ_AvoidActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AIJ_AvoidActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
		float currentTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
		float destoryTime = 1.f;
};
