// Fill out your copyright notice in the Description page of Project Settings.


#include "IJ_Player.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
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

	// ��Ʈ�ѷ��� ī�޶� ��Ī false
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

	// �÷��̾� �����Ʈ �⺻ ����
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// ī�޶� ��
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// ���� ī�޶�
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = true;

	// �÷��̾� Ű �Է� ������Ʈ
	playerMovement = CreateDefaultSubobject<UIJ_PlayerMovement>(TEXT("PlayerMovement"));

	// �÷��̾� ���¸ӽ� ������Ʈ
	playerFSM = CreateDefaultSubobject<UIJ_PlayerFSM>(TEXT("PlayerFSM"));

	// ������ ���� ������Ʈ
	damageManager = CreateDefaultSubobject<UIJ_DamageManager>(TEXT("DamagerManager"));

	// �ȵ���̵� Root
	androidBotRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("AndroidBotRootComp"));
	androidBotRootComp->SetupAttachment(RootComponent);

	// �ȵ���̵� �Ž�
	androidBot = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AndroidBot"));
	androidBot->SetupAttachment(androidBotRootComp);

	// �ȵ���̵� Left ���� ��ġ
	leftAndroidBotPos = CreateDefaultSubobject<USceneComponent>(TEXT("LeftAndroidBotPos"));
	leftAndroidBotPos->SetupAttachment(RootComponent);

	// �ȵ���̵� Right ���� ��ġ
	rightAndroidBotPos = CreateDefaultSubobject<USceneComponent>(TEXT("RightAndroidBotPos"));
	rightAndroidBotPos->SetupAttachment(RootComponent);

	// ���� ��ȣ�ۿ� ���� ��ġ
	interactionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionZone"));
	interactionZone->SetupAttachment(RootComponent);

	// AnimBP ���� �Ҵ�
	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnim(TEXT("AnimBlueprint'/Game/_IJ/PlayerAnim/BP_Player_Anim.BP_Player_Anim_C'"));

	// ���� �Ҵ� Ȯ��
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

	// ���� ȸ�� Ÿ�̹� ����
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

	// ��ϵ� ��������Ʈ �Լ� ��ü ����
	OnInputDelegate.Broadcast(PlayerInputComponent);
}

float AIJ_Player::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("")));
	//UE_LOG(LogTemp,Warning,TEXT("ddddddd"));
	
	if (playerMovement->bIsKillMontage != true)
	{
		// DamageCauser �� HP�� �� ���϶�� ���� ���� ���۽� End ��Ÿ�� ����, �ٸ� �Լ��� ����, Damage Causer ���� End ��Ÿ�� �����Ű��, bool�� �Ѱ��ֱ�
		enemy = DamageCauser;

		bIsTakeDamage = true;
		damageAmountValue = DamageAmount;

		SetDamage(DamageAmount);
	}
	return 0.0f;
}

void AIJ_Player::AndroidBot()
{
	// �ȵ���̵� ���Ʒ� ������ Sin ���� �Լ�
	FVector newLeftLocation = leftAndroidBotPos->GetRelativeLocation();
	FVector newRightLocation = rightAndroidBotPos->GetRelativeLocation();
	deltaHeight = (FMath::Sin(runningTime + (GetWorld()->DeltaTimeSeconds)) - FMath::Sin(runningTime));
	newLeftLocation.Z += deltaHeight * 20.f;
	newRightLocation.Z += deltaHeight * 20.f;
	runningTime += GetWorld()->DeltaTimeSeconds;
	leftAndroidBotPos->SetRelativeLocation(newLeftLocation);
	rightAndroidBotPos->SetRelativeLocation(newRightLocation);

	// ī�޶�� �ȵ���̵尡 ������� �� �ݴ� �������� �̵�
	if (bIsAndroidAttack == false)
	{
		if (FVector::Dist(GetFollowCamera()->GetComponentLocation(), leftAndroidBotPos->GetComponentLocation()) <= 350.f)
		{
			androidBot->SetRelativeLocation(FMath::Lerp(androidBot->GetRelativeLocation(), rightAndroidBotPos->GetRelativeLocation(), 2.5f * GetWorld()->DeltaTimeSeconds));
		}

		else
		{
			androidBot->SetRelativeLocation(FMath::Lerp(androidBot->GetRelativeLocation(), leftAndroidBotPos->GetRelativeLocation(), 2.5f * GetWorld()->DeltaTimeSeconds));
		}
	}
}
