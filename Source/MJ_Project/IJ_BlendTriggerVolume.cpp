// Fill out your copyright notice in the Description page of Project Settings.
#include "IJ_BlendTriggerVolume.h"
#include "Components/BoxComponent.h"
#include "IJ_Player.h"
#include "Camera/CameraActor.h"
#include "IJ_TopCamera.h"
#include "IJ_SideCamera.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AIJ_BlendTriggerVolume::AIJ_BlendTriggerVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("CameraProximityVolume"));
	OverlapVolume->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AIJ_BlendTriggerVolume::BeginPlay()
{
	Super::BeginPlay();

	topCamera = Cast<AIJ_TopCamera>(UGameplayStatics::GetActorOfClass(GetWorld(),AIJ_TopCamera::StaticClass()));
	sideCamera = Cast<AIJ_SideCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), AIJ_SideCamera::StaticClass()));
	player = Cast<AIJ_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), AIJ_Player::StaticClass()));
}

// 오버랩시
void AIJ_BlendTriggerVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (AIJ_Player* PlayerCharacterCheck = Cast<AIJ_Player>(OtherActor))
	{
		if (APlayerController* PlayerCharacterController = Cast<APlayerController>(PlayerCharacterCheck->GetController()))
		{
			// 카메라 블랜드
			PlayerCharacterController->SetViewTargetWithBlend(sideCamera, CameraBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);
			player->bIsSideView = true;

		}
	}
}

// 오버랩 종료
void AIJ_BlendTriggerVolume::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (AIJ_Player* PlayerCharacterCheck = Cast<AIJ_Player>(OtherActor))
	{
		if (APlayerController* PlayerCharacterController = Cast<APlayerController>(PlayerCharacterCheck->GetController()))
		{
			// 카메라 블랜드
			PlayerCharacterController->SetViewTargetWithBlend(PlayerCharacterController->GetPawn(), CameraBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);
			player->bIsSideView = false;

			Destroy(true);
		}
	}
}

// Called every frame
void AIJ_BlendTriggerVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}