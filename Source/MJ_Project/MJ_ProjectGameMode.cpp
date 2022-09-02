// Copyright Epic Games, Inc. All Rights Reserved.

#include "MJ_ProjectGameMode.h"
#include "MJ_ProjectCharacter.h"
#include "IJ_Player.h"
#include "IJ_PlayerMovement.h"
#include "Camera/CameraComponent.h"
#include <Kismet/GameplayStatics.h>
#include "UObject/ConstructorHelpers.h"



AMJ_ProjectGameMode::AMJ_ProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;

		bulletPoolSize = 100;
	}
}


void AMJ_ProjectGameMode::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AIJ_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), AIJ_Player::StaticClass()));

	// 1. Bullet�� ������ ������ ������ŭ Pool�� ����ش�.
	for (int32 i = 0; i < bulletPoolSize; ++i)
	{
		bullet = CreateBullet();

		AddBullet(bullet);
	}
}

// 4. Pool�� �� Bullet�� ��� ��Ȱ��ȭ��
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

// 3. Spawn��Ų Bullet�� �ϳ��� Pool�� �־���
// ����� �Ѿ��� �ٽ� Pool�� �־ ��Ȱ��
void AMJ_ProjectGameMode::AddBullet(AActor* bulletActor)
{
	bulletActor = bullet;
	// źâ�� �Ѿ� ����
	bulletPool.AddUnique(bullet);

	// �Ѿ� ��Ȱ��ȭ
	SetBulletActive(bullet, false);
}

// 5. pop ���Ѽ� Pool�� ��� Bullet�� �ϳ��� ������
AIJ_AndroidBotBullet* AMJ_ProjectGameMode::GetBullet()
{

	bullet = bulletPool.Pop();
	return bullet;
}

// 2. Bullet�� ������ ������Ŵ
AIJ_AndroidBotBullet* AMJ_ProjectGameMode::CreateBullet()
{
	// �Ѿ˰��忡�� �Ѿ� �����
	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	bullet = GetWorld()->SpawnActor<AIJ_AndroidBotBullet>(bulletFactory, FVector::ZeroVector, FRotator::ZeroRotator, Param);

	return bullet;
}
