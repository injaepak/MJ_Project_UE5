// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "YS_MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class MJ_PROJECT_API AYS_MonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	//생성자
	AYS_MonsterAIController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override; // 빙의
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;



	//감지 (버전변경으로 인한 상수참조값 변경)
	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*>& DetectedPawn); 
	 														  
	//시야거리
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightRadius = 1000;

	 //시야수명
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightAge = 5.f;

	 //놓치는시점
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AILoseSightRadius = AISightRadius + 50.f;

	//시야각
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AIFieldOfView = 180.f; 

	//퍼셉션 도우미
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		class UAISenseConfig_Sight* SightConfig;

	//인식 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool bIsPlayerDetected = false;

	//플레이어의 거리
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float DistanceToPlayer = 0.f;

private:

	UPROPERTY(BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
		class UBlackboardData* BlackboardData;

public:
	//FORCEINLINE UBlackboardData* GetUBlackboardData() const { return BlackboardData; }
};
