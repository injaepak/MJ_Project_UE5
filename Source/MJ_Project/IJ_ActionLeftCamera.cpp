// Fill out your copyright notice in the Description page of Project Settings.


#include "IJ_ActionLeftCamera.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AIJ_ActionLeftCamera::AIJ_ActionLeftCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	rootComp->SetupAttachment(RootComponent);

	sideCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SideCamera"));
	sideCamera->SetupAttachment(rootComp);
}

// Called when the game starts or when spawned
void AIJ_ActionLeftCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIJ_ActionLeftCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

