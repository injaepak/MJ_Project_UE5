// Fill out your copyright notice in the Description page of Project Settings.


#include "IJ_AndroidBotBullet.h"
#include "Components/BoxComponent.h"
#include "IJ_Player.h"
#include "MJ_ProjectGameMode.h"
#include "Camera/CameraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AIJ_AndroidBotBullet::AIJ_AndroidBotBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(boxComp);
}

// Called when the game starts or when spawned
void AIJ_AndroidBotBullet::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 캐스팅
	player = Cast<AIJ_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), AIJ_Player::StaticClass()));

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AIJ_AndroidBotBullet::OnCollisionEnter);
}

// Called every frame
void AIJ_AndroidBotBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector p = GetActorLocation() + dir * DeltaTime;
	SetActorLocation(p, true);
}

void AIJ_AndroidBotBullet::OnCollisionEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어가 아닐 때만
	if(OtherActor != player)
	{
		// 총알 오브젝트풀을 담고 있는 게임모드 캐스팅
		auto gameMode = Cast<AMJ_ProjectGameMode>(GetWorld()->GetAuthGameMode());
		
		gameMode->AddBullet(gameMode->bullet);

		// 충돌한 위치를 Vector 로 가져옴
		FVector hitResult = SweepResult.ImpactPoint;

		// 충돌한 위치에 이펙트 생성
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), fireEndFX,hitResult , boxComp->GetRelativeRotation());

		// 몬스터 캐스팅
		AYS_MonsterBase* const monster = Cast<AYS_MonsterBase>(OtherActor);

		// 만약 몬스터라면 몬스터 HP 감소
		if(OtherActor == monster)
		{
			if (monster->GetCurrentHealth() > 10)
			{
				monster->SetDamage(1.f);
			}
		}
	}

}
