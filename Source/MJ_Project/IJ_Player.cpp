// Fill out your copyright notice in the Description page of Project Settings.


#include "IJ_Player.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ChildActorComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "IJ_PlayerMovement.h"
#include "IJ_PlayerFSM.h"
#include "IJ_Player_AnimInstance.h"
#include "IJ_DamageManager.h"
#include <kismet/GameplayStatics.h>


// Sets default values
AIJ_Player::AIJ_Player()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// 컨트롤러와 카메라 매칭 false
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	ConstructorHelpers::FObjectFinder<UAnimMontage>BAM(TEXT("AnimMontage'/Game/_IJ/PlayerAnim/IJ_BaseCombo_01_Montage.IJ_BaseCombo_01_Montage'"));
	ConstructorHelpers::FObjectFinder<UAnimMontage>DAM(TEXT("AnimMontage'/Game/_IJ/PlayerAnim/IJ_DashAttack_01_Montage.IJ_DashAttack_01_Montage'"));

	if (BAM.Succeeded() || DAM.Succeeded())
	{
		baseAttackMontage = BAM.Object;
		dashAttackMontage = DAM.Object;
	}

	// 플레이어 무브먼트 기본 설정
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// 카메라 붐
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// 메인 카메라
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = true;

	// 카메라 액션
	MainCamChild = CreateDefaultSubobject<UChildActorComponent>(TEXT("MainCamChild"));
	MainCamChild->SetupAttachment(FollowCamera);
	MainCamChild->GetChildActor();

	SequenceCam = CreateDefaultSubobject<UCameraComponent>(TEXT("SequenceCam"));
	SequenceCam->SetupAttachment(RootComponent);

	SequenceChild = CreateDefaultSubobject<UChildActorComponent>(TEXT("SequenceChild"));
	SequenceChild->SetupAttachment(SequenceCam);
	SequenceChild->GetChildActor();

	// 플레이어 키 입력 컴포넌트
	playerMovement = CreateDefaultSubobject<UIJ_PlayerMovement>(TEXT("PlayerMovement"));

	// 플레이어 상태머신 컴포넌트
	playerFSM = CreateDefaultSubobject<UIJ_PlayerFSM>(TEXT("PlayerFSM"));

	// 데미지 관리 컴포넌트
	damageManager = CreateDefaultSubobject<UIJ_DamageManager>(TEXT("DamagerManager"));

	// 안드로이드 Root
	androidBotRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("AndroidBotRootComp"));
	androidBotRootComp->SetupAttachment(RootComponent);

	// 안드로이드 매쉬
	androidBot = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AndroidBot"));
	androidBot->SetupAttachment(androidBotRootComp);

	// 안드로이드 Left 고정 위치
	leftAndroidBotPos = CreateDefaultSubobject<USceneComponent>(TEXT("LeftAndroidBotPos"));
	leftAndroidBotPos->SetupAttachment(RootComponent);

	// 안드로이드 Right 고정 위치
	rightAndroidBotPos = CreateDefaultSubobject<USceneComponent>(TEXT("RightAndroidBotPos"));
	rightAndroidBotPos->SetupAttachment(RootComponent);

	// 몬스터 상호작용 고정 위치
	interactionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionZone"));
	interactionZone->SetupAttachment(RootComponent);

	// AnimBP 동적 할당
	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnim(TEXT("AnimBlueprint'/Game/_IJ/PlayerAnim/BP_Player_Anim.BP_Player_Anim_C'"));

	// 동적 할당 확인
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimClass(tempAnim.Class);
	}
}

// Called when the game starts or when spawned
void AIJ_Player::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AIJ_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AndroidBot();

	// 직전 회피 타이밍 로직
	if (bIsTakeDamage == true)
	{
		currentEvasionTime += DeltaTime;
		if (currentEvasionTime <= canEvasionTime)
		{
			bIsEvasion = true;
		}

		if (currentEvasionTime >= canEvasionTime)
		{
			bIsEvasion = false;
			damageManager->SetHP(damageAmountValue);
			currentEvasionTime = 0.f;
			bIsTakeDamage = false;
		}
	}
}

// Called to bind functionality to input
void AIJ_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 등록된 델리게이트 함수 전체 실행
	OnInputDelegate.Broadcast(PlayerInputComponent);
}

float AIJ_Player::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("")));
	//UE_LOG(LogTemp,Warning,TEXT("ddddddd"));

	if (playerMovement->bIsKillMontage != true)
	{
		AActor* MC = MainCamChild->GetChildActor();
		APlayerController* PlayerCharacterController = Cast<APlayerController>(GetController());
		PlayerCharacterController->SetViewTargetWithBlend(MC, 0.2f, EViewTargetBlendFunction::VTBlend_EaseInOut, 1.f);

		// DamageCauser 의 HP가 몇 이하라면 다음 공격 시작시 End 몽타주 실행, 다른 함수를 막음, Damage Causer 에게 End 몽타주 실행시키기, bool값 넘겨주기
		enemy = DamageCauser;

		bIsTakeDamage = true;
		damageAmountValue = DamageAmount;

		SetDamage(DamageAmount);
	}
	return 0.0f;
}

void AIJ_Player::AndroidBot()
{
	// 안드로이드 위아래 움직임 Sin 로직 함수
	FVector newLeftLocation = leftAndroidBotPos->GetRelativeLocation();
	FVector newRightLocation = rightAndroidBotPos->GetRelativeLocation();
	deltaHeight = (FMath::Sin(runningTime + (GetWorld()->DeltaTimeSeconds)) - FMath::Sin(runningTime));
	newLeftLocation.Z += deltaHeight * 20.f;
	newRightLocation.Z += deltaHeight * 20.f;
	runningTime += GetWorld()->DeltaTimeSeconds;
	leftAndroidBotPos->SetRelativeLocation(newLeftLocation);
	rightAndroidBotPos->SetRelativeLocation(newRightLocation);

	// 카메라와 안드로이드가 가까워질 때 반대 방향으로 이동
	if (FVector::Dist(GetFollowCamera()->GetComponentLocation(), leftAndroidBotPos->GetComponentLocation()) <= 350.f)
	{
		androidBot->SetRelativeLocation(FMath::Lerp(androidBot->GetRelativeLocation(), rightAndroidBotPos->GetRelativeLocation(), GetWorld()->DeltaTimeSeconds));
	}
	else
	{
		androidBot->SetRelativeLocation(FMath::Lerp(androidBot->GetRelativeLocation(), leftAndroidBotPos->GetRelativeLocation(), GetWorld()->DeltaTimeSeconds));
	}
}
