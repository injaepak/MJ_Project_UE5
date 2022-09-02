// Fill out your copyright notice in the Description page of Project Settings.

#include "YS_MonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "YS_MonsterBase.h"

AYS_MonsterAIController::AYS_MonsterAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BT(TEXT("BehaviorTree'/Game/YS/YS_Blueprint/AI/YS_BT_Monster.YS_BT_Monster'"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UBlackboardData>BD(TEXT("BlackboardData'/Game/YS/YS_Blueprint/AI/YS_BB_Monster.YS_BB_Monster'"));
	if (BD.Succeeded())
	{
		BlackboardData = BD.Object;
	}


	//�ۼ��� ������Ʈ �ҷ�����
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SIGHT_CONFIG"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PERCEPTION_COMPONENT")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	//-----------------�ν�ü ���� ���---------------------------//
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	//------------------------------------------------------------//

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AYS_MonsterAIController::OnPawnDetected);
}


void AYS_MonsterAIController::BeginPlay()
{
	Super::BeginPlay();


	//�������� üũ
	AYS_MonsterBase* const monster = Cast<AYS_MonsterBase>(GetPawn());
	GetBlackboardComponent()->SetValueAsBool(FName(TEXT("bIsBoss")), monster->bIsBoss);

}

void AYS_MonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	//����ó��
	if (InPawn == nullptr) return;

	if (UseBlackboard(BlackboardData, Blackboard))
	{
		if (RunBehaviorTree(BehaviorTree))
		{

		}
	}
}

void AYS_MonsterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

FRotator AYS_MonsterAIController::GetControlRotation() const
{

	//����ó��
	if (GetPawn() == nullptr)
	{
		return FRotator(0.f, 0.f, 0.f);
	}


	//����
	return FRotator(0.f,GetPawn()->GetActorRotation().Yaw,0.f);
}

void AYS_MonsterAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawn)
{

	auto PlayerCharacterRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	//���� ������ ���

	for (size_t i = 0; i < DetectedPawn.Num(); i++)
	{

		//�����忡�� bDetected�� ��������
		bool CurrentDetected = GetBlackboardComponent()->GetValueAsBool(FName(TEXT("bIsPlayerDetected")));

		if (DetectedPawn[i] == PlayerCharacterRef)
		{

			//�����忡 bDetected���� �̹� true�� �ٲ�������� false�� ����
			if (CurrentDetected == true)
			{
				bIsPlayerDetected = false;
				GetBlackboardComponent()->SetValueAsBool(FName(TEXT("bIsPlayerDetected")), bIsPlayerDetected);
			}
			//�����忡 bDetected���� false�� true�� ����
			else if (CurrentDetected == false)
			{

				bIsPlayerDetected = true;
				GetBlackboardComponent()->SetValueAsBool(FName(TEXT("bIsPlayerDetected")), bIsPlayerDetected);


			}

		}
	}
}
