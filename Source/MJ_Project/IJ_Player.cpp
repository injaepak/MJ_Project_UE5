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
#include "IJ_ActionCameraManager.h"
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
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength, 200.f, 400.f);
	CameraBoom->bUsePawnControlRotation = true;

	// 메인 카메라
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = true;

	// 카메라 액션
	cameraChild_L = CreateDefaultSubobject<UChildActorComponent>(TEXT("CameraChild_L"));
	cameraChild_L->SetupAttachment(FollowCamera);

	cameraChild_M = CreateDefaultSubobject<UChildActorComponent>(TEXT("CameraChild_M"));
	cameraChild_M->SetupAttachment(FollowCamera);

	cameraChild_R = CreateDefaultSubobject<UChildActorComponent>(TEXT("CameraChild_R"));
	cameraChild_R->SetupAttachment(FollowCamera);

	// 카메라 BlendCollision
	cameraCollision_L = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraCollision_L"));
	cameraCollision_L->SetupAttachment(RootComponent);
	cameraCollision_M = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraCollision_M"));
	cameraCollision_M->SetupAttachment(RootComponent);
	cameraCollision_R = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraCollision_R"));
	cameraCollision_R->SetupAttachment(RootComponent);

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
	cameraCollision_L->OnComponentBeginOverlap.AddDynamic(this, &AIJ_Player::OnCollisionEnter);
	cameraCollision_M->OnComponentBeginOverlap.AddDynamic(this, &AIJ_Player::OnCollisionEnter);
	cameraCollision_R->OnComponentBeginOverlap.AddDynamic(this, &AIJ_Player::OnCollisionEnter);
	cameraCollision_L->OnComponentEndOverlap.AddDynamic(this, &AIJ_Player::EndOverlap);
	cameraCollision_M->OnComponentEndOverlap.AddDynamic(this, &AIJ_Player::EndOverlap);
	cameraCollision_R->OnComponentEndOverlap.AddDynamic(this, &AIJ_Player::EndOverlap);

	actionCam = Cast<AIJ_ActionCameraManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AIJ_ActionCameraManager::StaticClass()));
}

// Called every frame
void AIJ_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AndroidBot();

	Evasion();

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

void AIJ_Player::Evasion()
{
	// 직전 회피 타이밍 로직
	if (bIsTakeDamage == true)
	{
		currentEvasionTime += GetWorld()->DeltaTimeSeconds;
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

void AIJ_Player::OnCollisionEnter(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsSideView == false)
	{
		if (OverlappedComp == cameraCollision_R && OverlappedComp == cameraCollision_L)
		{
			APlayerController* PlayerCharacterController = Cast<APlayerController>(GetController());
			PlayerCharacterController->SetViewTargetWithBlend(PlayerCharacterController->GetPawn(), 1.f, EViewTargetBlendFunction::VTBlend_Linear);
		}

		else if (OverlappedComp == cameraCollision_L && OverlappedComp != cameraCollision_R)
		{
			AActor* MC = cameraChild_R->GetChildActor();
			UCameraComponent* TargetCam = MC->FindComponentByClass<UCameraComponent>();
			if (TargetCam)
			{
				TargetCam->SetConstraintAspectRatio(false);
			}
			APlayerController* PlayerCharacterController = Cast<APlayerController>(GetController());
			PlayerCharacterController->SetViewTargetWithBlend(MC, 1.f, EViewTargetBlendFunction::VTBlend_Linear);
			bIsCameraBlending = true;
		}
		else if (OverlappedComp == cameraCollision_R && OverlappedComp != cameraCollision_L)
		{
			AActor* MC = cameraChild_L->GetChildActor();
			UCameraComponent* TargetCam = MC->FindComponentByClass<UCameraComponent>();
			if (TargetCam)
			{
				TargetCam->SetConstraintAspectRatio(false);
			}
			APlayerController* PlayerCharacterController = Cast<APlayerController>(GetController());
			PlayerCharacterController->SetViewTargetWithBlend(MC, 1.f, EViewTargetBlendFunction::VTBlend_Linear);
			bIsCameraBlending = true;
		}
	}

}

void AIJ_Player::EndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndexs)
{
	if (bIsSideView == false)
	{
		if (OverlappedComp == cameraCollision_L && OverlappedComp != cameraCollision_R)
		{
			APlayerController* PlayerCharacterController = Cast<APlayerController>(GetController());
			PlayerCharacterController->SetViewTargetWithBlend(PlayerCharacterController->GetPawn(), 1.f, EViewTargetBlendFunction::VTBlend_Linear);
			bIsCameraBlending = false;
		}
		else if (OverlappedComp == cameraCollision_R && OverlappedComp != cameraCollision_L)
		{
			APlayerController* PlayerCharacterController = Cast<APlayerController>(GetController());
			PlayerCharacterController->SetViewTargetWithBlend(PlayerCharacterController->GetPawn(), 1.f, EViewTargetBlendFunction::VTBlend_Linear);
			bIsCameraBlending = false;
		}
		else if (OverlappedComp == cameraCollision_R && OverlappedComp == cameraCollision_L)
		{
			APlayerController* PlayerCharacterController = Cast<APlayerController>(GetController());
			PlayerCharacterController->SetViewTargetWithBlend(PlayerCharacterController->GetPawn(), 0.2f, EViewTargetBlendFunction::VTBlend_Linear);
			bIsCameraBlending = false;
		}
	}
	
}
