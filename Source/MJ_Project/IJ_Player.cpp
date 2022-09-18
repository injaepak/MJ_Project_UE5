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
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength, 200.f, 500.f);
	CameraBoom->bUsePawnControlRotation = true;

	// ���� ī�޶�
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = true;

	// ī�޶� �׼�
	cameraChild_L = CreateDefaultSubobject<UChildActorComponent>(TEXT("CameraChild_L"));
	cameraChild_L->SetupAttachment(FollowCamera);

	cameraChild_M = CreateDefaultSubobject<UChildActorComponent>(TEXT("CameraChild_M"));
	cameraChild_M->SetupAttachment(FollowCamera);

	cameraChild_R = CreateDefaultSubobject<UChildActorComponent>(TEXT("CameraChild_R"));
	cameraChild_R->SetupAttachment(FollowCamera);

	// ī�޶� BlendCollision
	cameraCollision_L = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraCollision_L"));
	cameraCollision_L->SetupAttachment(RootComponent);
	cameraCollision_M = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraCollision_M"));
	cameraCollision_M->SetupAttachment(RootComponent);
	cameraCollision_R = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraCollision_R"));
	cameraCollision_R->SetupAttachment(RootComponent);

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
	cameraCollision_L->OnComponentBeginOverlap.AddDynamic(this, &AIJ_Player::OnCollisionEnter);
	cameraCollision_M->OnComponentBeginOverlap.AddDynamic(this, &AIJ_Player::OnCollisionEnter);
	cameraCollision_R->OnComponentBeginOverlap.AddDynamic(this, &AIJ_Player::OnCollisionEnter);
	cameraCollision_L->OnComponentEndOverlap.AddDynamic(this, &AIJ_Player::EndOverlap);
	cameraCollision_M->OnComponentEndOverlap.AddDynamic(this, &AIJ_Player::EndOverlap);
	cameraCollision_R->OnComponentEndOverlap.AddDynamic(this, &AIJ_Player::EndOverlap);
	
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
		/*AActor* MC = MainCamChild->GetChildActor();
		APlayerController* PlayerCharacterController = Cast<APlayerController>(GetController());
		PlayerCharacterController->SetViewTargetWithBlend(MC, 0.2f, EViewTargetBlendFunction::VTBlend_EaseInOut, 1.f);*/
		

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
	if (FVector::Dist(GetFollowCamera()->GetComponentLocation(), leftAndroidBotPos->GetComponentLocation()) <= 350.f)
	{
		androidBot->SetRelativeLocation(FMath::Lerp(androidBot->GetRelativeLocation(), rightAndroidBotPos->GetRelativeLocation(), GetWorld()->DeltaTimeSeconds));
	}
	else
	{
		androidBot->SetRelativeLocation(FMath::Lerp(androidBot->GetRelativeLocation(), leftAndroidBotPos->GetRelativeLocation(), GetWorld()->DeltaTimeSeconds));
	}
}

void AIJ_Player::OnCollisionEnter(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComp == cameraCollision_R && OverlappedComp == cameraCollision_L)
	{
		AActor* MC = cameraChild_M->GetChildActor();
		APlayerController* PlayerCharacterController = Cast<APlayerController>(GetController());
		PlayerCharacterController->SetViewTargetWithBlend(MC, 0.25f, EViewTargetBlendFunction::VTBlend_EaseIn, 2.f, true);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("")));
		UE_LOG(LogTemp, Warning, TEXT("GO_M"));
	}

	else if (OverlappedComp == cameraCollision_L && OverlappedComp != cameraCollision_R)
	{
		AActor* MC = cameraChild_R->GetChildActor();
		APlayerController* PlayerCharacterController = Cast<APlayerController>(GetController());
		PlayerCharacterController->SetViewTargetWithBlend(MC, 0.25f, EViewTargetBlendFunction::VTBlend_EaseIn, 2.f, true);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("")));
		UE_LOG(LogTemp,Warning,TEXT("GO_R"));
	}
	else if (OverlappedComp == cameraCollision_R && OverlappedComp != cameraCollision_L)
	{
		AActor* MC = cameraChild_L->GetChildActor();
		APlayerController* PlayerCharacterController = Cast<APlayerController>(GetController());
		PlayerCharacterController->SetViewTargetWithBlend(MC, 0.25f, EViewTargetBlendFunction::VTBlend_EaseIn, 2.f, true);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("")));
		UE_LOG(LogTemp, Warning, TEXT("GO_L"));
	}
}

void AIJ_Player::EndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndexs)
{
	if (OverlappedComp == cameraCollision_L && OverlappedComp != cameraCollision_R)
	{
		AActor* MC = cameraChild_M->GetChildActor();
		APlayerController* PlayerCharacterController = Cast<APlayerController>(GetController());
		PlayerCharacterController->SetViewTargetWithBlend(MC, 0.25f, EViewTargetBlendFunction::VTBlend_EaseIn, 2.f, true);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("")));
		UE_LOG(LogTemp, Warning, TEXT("GO_M1"));
	}
	else if (OverlappedComp == cameraCollision_R && OverlappedComp != cameraCollision_L)
	{
		AActor* MC = cameraChild_M->GetChildActor();
		APlayerController* PlayerCharacterController = Cast<APlayerController>(GetController());
		PlayerCharacterController->SetViewTargetWithBlend(MC, 0.25f, EViewTargetBlendFunction::VTBlend_EaseIn, 2.f, true);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("")));
		UE_LOG(LogTemp, Warning, TEXT("GO_M2"));
	}
	else if (OverlappedComp == cameraCollision_R && OverlappedComp == cameraCollision_L)
	{
		AActor* MC = cameraChild_M->GetChildActor();
		APlayerController* PlayerCharacterController = Cast<APlayerController>(GetController());
		PlayerCharacterController->SetViewTargetWithBlend(MC, 0.25f, EViewTargetBlendFunction::VTBlend_EaseIn, 2.f, true);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("")));
		UE_LOG(LogTemp, Warning, TEXT("GO_M3"));
	}
}
