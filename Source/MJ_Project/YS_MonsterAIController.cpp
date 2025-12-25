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


	//퍼셉션 컴포넌트 불러오기
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SIGHT_CONFIG"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PERCEPTION_COMPONENT")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	//-----------------인식체 종류 허용---------------------------//
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

	//비헤이비어트리 실행
	RunBehaviorTree(BehaviorTree);

	//블랙보드 실행
	UBlackboardComponent* tempBlackboard = Blackboard;
	UseBlackboard(BlackboardData, tempBlackboard);
	this->Blackboard = tempBlackboard;


	//보스인지 체크
	AYS_MonsterBase* const monster = Cast<AYS_MonsterBase>(GetPawn());
	GetBlackboardComponent()->SetValueAsBool(FName(TEXT("bIsBoss")), monster->GetIsBoss());

}

void AYS_MonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	////에외처리
	if (InPawn == nullptr) return;

	
	
}

void AYS_MonsterAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

FRotator AYS_MonsterAIController::GetControlRotation() const
{

	Super::GetControlRotation();

	//예외처리
	if (GetPawn() == nullptr)
	{
		return FRotator(0.f, 0.f, 0.f);
	}


	//정상값
	return FRotator(0.f,GetPawn()->GetActorRotation().Yaw,0.f);
}

void AYS_MonsterAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawn)
{

	auto PlayerCharacterRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	//폰이 들어왔을 경우

	for (size_t i = 0; i < DetectedPawn.Num(); i++)
	{

		//블랙보드에서 bDetected값 가져오기
		bool CurrentDetected = GetBlackboardComponent()->GetValueAsBool(FName(TEXT("bIsPlayerDetected")));

		if (DetectedPawn[i] == PlayerCharacterRef)
		{

			//블랙보드에 bDetected값이 이미 true로 바뀌어있으면 false로 변경
			if (CurrentDetected == true)
			{
				bIsPlayerDetected = false;
				GetBlackboardComponent()->SetValueAsBool(FName(TEXT("bIsPlayerDetected")), bIsPlayerDetected);
			}
			//블랙보드에 bDetected값이 false면 true로 변경
			else if (CurrentDetected == false)
			{

				bIsPlayerDetected = true;
				GetBlackboardComponent()->SetValueAsBool(FName(TEXT("bIsPlayerDetected")), bIsPlayerDetected);


			}

		}
	}
}
