// Fill out your copyright notice in the Description page of Project Settings.


#include "IJ_ActionRightCamera.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AIJ_ActionRightCamera::AIJ_ActionRightCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	rootComp->SetupAttachment(RootComponent);

	sideCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SideCamera"));
	sideCamera->SetupAttachment(rootComp);

	sideCamera->SetConstraintAspectRatio(false);
}

// Called when the game starts or when spawned
void AIJ_ActionRightCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIJ_ActionRightCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

