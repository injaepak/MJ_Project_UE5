// Fill out your copyright notice in the Description page of Project Settings.


#include "IJ_SideCamera.h"
#include "IJ_Player.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AIJ_SideCamera::AIJ_SideCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	rootComp->SetupAttachment(RootComponent);

	sideCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SideCamera"));
	sideCamera->SetupAttachment(rootComp);
}

// Called when the game starts or when spawned
void AIJ_SideCamera::BeginPlay()
{
	Super::BeginPlay();

	myLoc = rootComp->GetComponentLocation();

	myLocX = rootComp->GetComponentLocation().X;
	
	player = Cast<AIJ_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), AIJ_Player::StaticClass()));
}

// Called every frame
void AIJ_SideCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (player)
	{
		targetLocX = player->GetActorLocation().X;

		myLocX = FMath::Lerp(myLocX, targetLocX, 5.f * GetWorld()->DeltaTimeSeconds);

		// 카메라 X축 이동
		rootComp->SetRelativeLocation(FVector(myLocX, myLoc.Y, myLoc.Z));
	}
}

