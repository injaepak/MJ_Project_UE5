// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "YS_MonsterBase.h"
#include "IJ_AndroidBotBullet.generated.h"

UCLASS()
class MJ_PROJECT_API AIJ_AndroidBotBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIJ_AndroidBotBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		class UBoxComponent* boxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		class AIJ_Player* player;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		class AYS_MonsterBase* mb;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		FVector dir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		float speed = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setting)
		float fireDistance = 10000.f;

	UPROPERTY(EditDefaultsOnly, Category = Fire)
		class UNiagaraSystem* fireEndFX;

public:

	UFUNCTION()
		void OnCollisionEnter(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	UPROPERTY(EditDefaultsOnly, Category = Fire)
	float damageAmountValue = 3.f;

};
