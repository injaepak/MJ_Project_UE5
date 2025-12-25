// Copyright Epic Games, Inc. All Rights Reserved.

#include "MJ_ProjectGameMode.h"
#include "MJ_ProjectCharacter.h"
#include "IJ_Player.h"
#include "IJ_PlayerMovement.h"
#include "Camera/CameraComponent.h"
#include <Kismet/GameplayStatics.h>
#include "UObject/ConstructorHelpers.h"
#include "Components/AudioComponent.h"



AMJ_ProjectGameMode::AMJ_ProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;

		bulletPoolSize = 100;
	}

	BackgroundMusicComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AUDIOCOMP"));

	static ConstructorHelpers::FObjectFinder<USoundBase>BGM(TEXT("SoundWave'/Game/YS/Sound/Midnight_Trace_-_Jimena_Contreras.Midnight_Trace_-_Jimena_Contreras'"));
	if (BGM.Succeeded())
	{
		BackgroundMusic = BGM.Object;
	}

	BackgroundMusicComponent->SetSound(BackgroundMusic);
}


void AMJ_ProjectGameMode::BeginPlay()
{
	Super::BeginPlay();


	BackgroundMusicComponent->Play();




	player = Cast<AIJ_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), AIJ_Player::StaticClass()));

	// 1. Bullet을 생성해 설정한 갯수만큼 Pool에 담아준다.
	for (int32 i = 0; i < bulletPoolSize; ++i)
	{
		bullet = CreateBullet();

		AddBullet(bullet);
	}
}

void AMJ_ProjectGameMode::ChangeBackgroundSound(USoundBase* NewSound)
{
	BackgroundMusicComponent->Stop();
	BackgroundMusicComponent->SetSound(NewSound);
	BackgroundMusicComponent->Play();
}

// 4. Pool에 들어간 Bullet은 모두 비활성화함
void AMJ_ProjectGameMode::SetBulletActive(AActor* bulletActor, bool isActive)
{
	bulletActor = bullet;

	bullet->SetActorLocation(player->androidBot->GetComponentLocation());
	bullet->SetActorRotation(player->androidBot->GetComponentRotation());

	bullet->dir = (player->GetFollowCamera()->GetRelativeLocation() + player->GetFollowCamera()->GetForwardVector());
	bullet->dir.Normalize();
	bullet->dir *= bullet->speed;

	bullet->SetActorHiddenInGame(!isActive);
	bullet->SetActorEnableCollision(isActive);
	bullet->SetActorTickEnabled(isActive);

}

// 3. Spawn시킨 Bullet을 하나씩 Pool에 넣어줌
// 사용한 총알은 다시 Pool에 넣어서 재활용
void AMJ_ProjectGameMode::AddBullet(AActor* bulletActor)
{
	bulletActor = bullet;
	// 탄창에 총알 장착
	bulletPool.AddUnique(bullet);

	// 총알 비활성화
	SetBulletActive(bullet, false);
}

// 5. pop 시켜서 Pool에 담김 Bullet을 하나씩 가져옴
AIJ_AndroidBotBullet* AMJ_ProjectGameMode::GetBullet()
{

	bullet = bulletPool.Pop();
	return bullet;
}

// 2. Bullet을 레벨에 스폰시킴
AIJ_AndroidBotBullet* AMJ_ProjectGameMode::CreateBullet()
{
	// 총알공장에서 총알 만들기
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	bullet = GetWorld()->SpawnActor<AIJ_AndroidBotBullet>(bulletFactory, FVector::ZeroVector, FRotator::ZeroRotator, Param);

	return bullet;
}
