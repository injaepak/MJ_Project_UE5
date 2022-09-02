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

	// �Ѿ� Ȱ��ȭ��Ű�� �Լ�
	void SetBulletActive(AActor* bulletActor, bool isActive);

	// źâ�� �Ѿ��� �߰����ִ� �Լ�
	void AddBullet(AActor* bulletActor);

	// źâ(Ǯ) ���� �Ѿ��� �ϳ� ������
	AIJ_AndroidBotBullet* GetBullet();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Setting)
		class AIJ_Player* player;

	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
		int bulletPoolSize;

	UPROPERTY(VisibleAnywhere, Category = "Bullet")
		TArray<AIJ_AndroidBotBullet*> bulletPool;

	UPROPERTY(VisibleAnywhere, Category = "Bullet")
		AIJ_AndroidBotBullet* bullet;

	// ����
	UPROPERTY(EditDefaultsOnly, Category = "BulletClass")
		TSubclassOf<class AIJ_AndroidBotBullet> bulletFactory;

private:
	AIJ_AndroidBotBullet* CreateBullet();
};



