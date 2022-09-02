// Fill out your copyright notice in the Description page of Project Settings.


#include "IJ_TopCamera.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AIJ_TopCamera::AIJ_TopCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	rootComp->SetupAttachment(RootComponent);

	topCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopCamera"));
	topCamera->SetupAttachment(rootComp);
}

// Called when the game starts or when spawned
void AIJ_TopCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIJ_TopCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

