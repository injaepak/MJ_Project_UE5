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
	//������
	AYS_MonsterAIController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override; // ����
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;



	//���� (������������ ���� ��������� ����)
	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*>& DetectedPawn); 
	 														  
	//�þ߰Ÿ�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightRadius = 1000;

	 //�þ߼���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightAge = 5.f;

	 //��ġ�½���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AILoseSightRadius = AISightRadius + 50.f;

	//�þ߰�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AIFieldOfView = 180.f; 

	//�ۼ��� �����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		class UAISenseConfig_Sight* SightConfig;

	//�ν� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		bool bIsPlayerDetected = false;

	//�÷��̾��� �Ÿ�
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
