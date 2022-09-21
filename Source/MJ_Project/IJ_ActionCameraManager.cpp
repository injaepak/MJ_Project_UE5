// Fill out your copyright notice in the Description page of Project Settings.


#include "IJ_ActionCameraManager.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AIJ_ActionCameraManager::AIJ_ActionCameraManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	rootComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AIJ_ActionCameraManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIJ_ActionCameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

