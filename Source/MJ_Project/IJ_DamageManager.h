// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IJ_DamageManager.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MJ_PROJECT_API UIJ_DamageManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UIJ_DamageManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class AIJ_Player* me;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float maxHp;

	bool isDead;

	inline float GetHP() { return hp; }

	void SetHP(float Damage);
	void Die();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		float currentTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		float maxCurrentTime = 0.5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		float slowTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		float maxSlowTime = 0.1f;

	FVector myPos;
	FVector playerPos;
	FVector knockbackPos;
	float knockbackValue = 200.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		bool bDefenceSlowOn = false;
};
