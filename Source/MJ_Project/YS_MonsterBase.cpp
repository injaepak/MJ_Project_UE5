// Fill out your copyright notice in the Description page of Project Settings.

#include "YS_MonsterBase.h"
#include "IJ_Player.h"
#include "IJ_PlayerMovement.h"
#include "IJ_Player_AnimInstance.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "YS_MonsterWidget.h"
#include "NiagaraFunctionLibrary.h"

AYS_MonsterBase::AYS_MonsterBase()
{

	PrimaryActorTick.bCanEverTick = true;

	lockOn = CreateDefaultSubobject<UWidgetComponent>(TEXT("LOCKON"));
	lockOn->bHiddenInGame = 1;
	lockOn->SetWidgetSpace(EWidgetSpace::Screen);
	lockOn->SetupAttachment(GetMesh());


	static ConstructorHelpers::FClassFinder<UUserWidget> LO(TEXT("WidgetBlueprint'/Game/TheTargetingSystem/UI/Widgets/W_LockOn.W_LockOn_C'"));
	if (LO.Succeeded())
	{
		lockOn->SetWidgetClass(LO.Class);
	}

	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget>UW(TEXT("WidgetBlueprint'/Game/YS/YS_Blueprint/Widget/YS_WBP_MonsterHpBar.YS_WBP_MonsterHpBar_C'"));
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
		HpBar->SetDrawSize(FVector2D(80.f, 10.f));
	}


	
}

void AYS_MonsterBase::BeginPlay()
{
	Super::BeginPlay();

	directionCal = GetActorLocation();
	
	player = Cast<AIJ_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), AIJ_Player::StaticClass()));
	playerAnim = Cast<UIJ_Player_AnimInstance>(player->GetMesh()->GetAnimInstance());

	

	HpBar->InitWidget();
	auto HpWidget = Cast<UYS_MonsterWidget>(HpBar->GetUserWidgetObject());
	if (HpWidget)
	{
		HpWidget->BindHp(this);
	}

	OnHpChanged.Broadcast();

}


void AYS_MonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	KnockBackUpdate(DeltaTime);
	UE_LOG(LogTemp, Log, TEXT("%f"),currentHealth);

	if (GetDistanceTo(player) < 1000.f)
	{
		HpBar->SetVisibility(true);
	}
	else
	{
		HpBar->SetVisibility(false);
	}
}




UAnimMontage* AYS_MonsterBase::GetBasicAttackMontage()
{
	int randomInt = FMath::RandRange(0, basicAttackMontage.Max() - 2);

	return basicAttackMontage[randomInt];
}



void AYS_MonsterBase::SetDamage(float Damage)
{
	currentHealth -= Damage;
	OnHpChanged.Broadcast();
}

void AYS_MonsterBase::Attack(UAnimMontage* AttackMontage)
{
	
	if (!(GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontage)))
	{

		PlayAnimMontage(AttackMontage);
		
	}
}


void AYS_MonsterBase::KnockBack(AActor* Player)
{
	
	ACharacter* PlayerCharacter = Cast<ACharacter>(Player);
	//----------------------------------------------계산------------------------------------------------------------------------
	FVector findPlayerVec = PlayerCharacter->GetActorForwardVector();
	directionCal = GetActorLocation() + findPlayerVec * knockBackDintance;
	//---------------------------------------------------------------------------------------------------------------------------

	
}

void AYS_MonsterBase::KnockBackUpdate(float delta)
{
	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(DamageMontage))
	{
		FinalKnockBackLocation =
			FMath::VInterpTo(GetActorLocation(), directionCal, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 10.f);
		SetActorLocation(FVector(FinalKnockBackLocation.X, FinalKnockBackLocation.Y, GetActorLocation().Z));
	}
}

float AYS_MonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	KnockBack(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayAnimMontage(DamageMontage);


	SetDamage(DamageAmount);

	// 인터렉션
	if (GetHpRatio() <= 0.1)// 만약 체력이 일정수준 이하면 실행
	{
		SetActorLocation(player->interactionZone->GetComponentLocation());
		SetActorRotation(player->interactionZone->GetComponentRotation());
		player->playerMovement->bIsKillMontage = true;
		playerAnim->kill();
		if (playerAnim->Montage_IsPlaying(playerAnim->killMontage))
		{
			Die = true;
			Death();
		}
		
	}
	OnHpChanged.Broadcast();
	return 0.0f;
}

UFUNCTION(BlueprintCallable)
void AYS_MonsterBase::Death()
{
	
}
