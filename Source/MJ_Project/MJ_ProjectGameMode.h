// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IJ_AndroidBotBullet.h"
#include "GameFramework/GameModeBase.h"
#include "MJ_ProjectGameMode.generated.h"

UCLASS(minimalapi)
class AMJ_ProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMJ_ProjectGameMode();

	virtual void BeginPlay() override;

	// 총알 활성화시키는 함수
	void SetBulletActive(AActor* bulletActor, bool isActive);

	// 탄창에 총알을 추가해주는 함수
	void AddBullet(AActor* bulletActor);

	// 탄창(풀) 에서 총알을 하나 빼오기
	AIJ_AndroidBotBullet* GetBullet();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		class AIJ_Player* player;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
		int bulletPoolSize;

	UPROPERTY(VisibleAnywhere, Category = "Bullet")
		TArray<AIJ_AndroidBotBullet*> bulletPool;

	UPROPERTY(VisibleAnywhere, Category = "Bullet")
		AIJ_AndroidBotBullet* bullet;

	// 공장
	UPROPERTY(EditDefaultsOnly, Category = "BulletClass")
		TSubclassOf<class AIJ_AndroidBotBullet> bulletFactory;

private:
	AIJ_AndroidBotBullet* CreateBullet();
};



