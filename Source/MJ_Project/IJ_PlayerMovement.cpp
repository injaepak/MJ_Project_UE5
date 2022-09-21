// Fill out your copyright notice in the Description page of Project Settings.

#include "IJ_PlayerMovement.h"
#include "IJ_Player.h"
#include "IJ_PlayerFSM.h"
#include "IJ_Player_AnimInstance.h"
#include "IJ_AvoidActor.h"
#include "IJ_AndroidBotBullet.h"
#include <kismet/GameplayStatics.h>
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "MJ_ProjectGameMode.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values for this component's properties
UIJ_PlayerMovement::UIJ_PlayerMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

// Called when the game starts
void UIJ_PlayerMovement::BeginPlay()
{
	Super::BeginPlay();

	// 점프를 수 조절
	me->JumpMaxCount = 2;

	me->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void UIJ_PlayerMovement::InitializeComponent()
{
	me = Cast<AIJ_Player>(GetOwner());

	// 델리게이트에 내 함수를 등록하고 싶다.
	// InitializeComponent 로 BeginPlay 이전에 초기화 등록해준다.
	if (me)
	{
		me->OnInputDelegate.AddUObject(this, &UIJ_PlayerMovement::SetupPlayerInputComponent);
	}
}

// Called every frame
void UIJ_PlayerMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 플레이어가 Run 상태일 때
	if (me->playerFSM->m_state == EPlayerState::Run)
	{
		// Velocity 가 0 이라면
		if (FVector(me->GetVelocity()).Size() == 0.f)
		{
			// Idle 상태로 변경한다.
			me->playerFSM->m_state = EPlayerState::Idle;
		}
	}

	// 만약 Defence 상태라면 MaxWalkSpeed = 200
	if (me->playerFSM->m_state == EPlayerState::Defence)
	{
		me->GetCharacterMovement()->MaxWalkSpeed = 200;
	}

	// 직전 회피상태가 아니라면, 초기화한다.
	if (me->bIsEvasion == false)
	{
		// 월드 슬로우, 카매라릭, 매쉬 키기, 이펙트 끄기
		GetWorld()->GetWorldSettings()->SetTimeDilation(1.f);
		me->GetCameraBoom()->CameraLagSpeed = 10.f;
		me->GetMesh()->SetHiddenInGame(false);
		me->GetMesh()->GetChildComponent(0)->SetHiddenInGame(false);
		me->GetMesh()->GetChildComponent(1)->SetVisibility(false);
	}

	if ((me->playerFSM->m_state == EPlayerState::RunJump || me->playerFSM->m_state == EPlayerState::Jump) && me->playerFSM->anim->bIsInAir == false)
	{
		if (me->playerFSM->m_state == EPlayerState::RunJump)
		{
			me->playerFSM->m_state = EPlayerState::Run;
		}
		else
		{
			me->playerFSM->m_state = EPlayerState::Idle;
		}
	}
	dashCurrentTime += DeltaTime;
}

// 입력 함수 키 바인딩
void UIJ_PlayerMovement::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &UIJ_PlayerMovement::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &UIJ_PlayerMovement::StopJumping);
	PlayerInputComponent->BindAction("Dash", IE_DoubleClick, this, &UIJ_PlayerMovement::Dash);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &UIJ_PlayerMovement::Attack);
	PlayerInputComponent->BindAction("DefenceOn", IE_Pressed, this, &UIJ_PlayerMovement::DefenceOn);
	PlayerInputComponent->BindAction("DefenceOff", IE_Released, this, &UIJ_PlayerMovement::StopDefence);
	PlayerInputComponent->BindAction("AndroidBotAttackOn", IE_Pressed, this, &UIJ_PlayerMovement::AndroidBotAttackOn);
	PlayerInputComponent->BindAction("AndroidBotAttackOff", IE_Released, this, &UIJ_PlayerMovement::AndroidBotAttackOff);

	PlayerInputComponent->BindAxis("MoveForward", this, &UIJ_PlayerMovement::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &UIJ_PlayerMovement::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &UIJ_PlayerMovement::Turn);
	PlayerInputComponent->BindAxis("TurnRate", this, &UIJ_PlayerMovement::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &UIJ_PlayerMovement::LookUp);
	PlayerInputComponent->BindAxis("LookUpRate", this, &UIJ_PlayerMovement::LookUpAtRate);
	PlayerInputComponent->BindAxis("AndroidBotFire", this, &UIJ_PlayerMovement::AndroidBotFire);
}

void UIJ_PlayerMovement::TurnAtRate(float Rate)
{
	// 컨트롤러의 Yaw 축 방향의 회전속도
	me->AddControllerYawInput(Rate * me->BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void UIJ_PlayerMovement::LookUpAtRate(float Rate)
{
	// 컨트롤러의 Pitch 축 방향의 회전속도
	me->AddControllerPitchInput(Rate * me->BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void UIJ_PlayerMovement::Turn(float Rate)
{
	// Yaw 축 방향 회전
	me->AddControllerYawInput(Rate);
}

void UIJ_PlayerMovement::LookUp(float Rate)
{
	// Pitch 축 방향 회전
	me->AddControllerPitchInput(Rate);
}

void UIJ_PlayerMovement::MoveForward(float Value)
{

	// 컨트롤러가 있고, 값이 0 이 아니고, FinishMontage 가 실행되고 있지 않을 때
	if ((me->Controller != nullptr) && (Value != 0.0f) && (bIsKillMontage == false))
	{
		// Run 상태라면 Speed 1000
		if (me->playerFSM->m_state == EPlayerState::Run)
		{
			me->GetCharacterMovement()->MaxWalkSpeed = 1000.f;
		}

		// Idle, Defence 상태라면 Speed 200
		else if (me->playerFSM->m_state == EPlayerState::Idle)
		{
			me->GetCharacterMovement()->MaxWalkSpeed = 200.f;
		}

		if (me->bIsSideView == false)
		{
			const FRotator Rotation = me->Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			me->AddMovementInput(Direction, Value);
		}
	}
}

void UIJ_PlayerMovement::MoveRight(float Value)
{
	// 컨트롤러가 있고, 값이 0 이 아니고, FinishMontage 가 실행되고 있지 않을 때
	if ((me->Controller != nullptr) && (Value != 0.0f) && (bIsKillMontage == false))
	{
		// Run 상태라면 Speed 1000
		if (me->playerFSM->m_state == EPlayerState::Run)
		{
			me->GetCharacterMovement()->MaxWalkSpeed = 1000.f;
		}

		// Idle, Defence 상태라면 Speed 200
		else if (me->playerFSM->m_state == EPlayerState::Idle || me->playerFSM->m_state == EPlayerState::Defence)
		{
			me->GetCharacterMovement()->MaxWalkSpeed = 200;
		}

		// SideView 가 아니라면 컨트롤러의 로테이션값으로 움직인다.
		if (me->bIsSideView == false)
		{
			const FRotator Rotation = me->Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			me->AddMovementInput(Direction, Value);
		}
		// SideView 라면 매쉬의 로테이션 값으로 움직인다.
		else
		{
			const FRotator Rotation = me->GetMesh()->GetRelativeRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			me->AddMovementInput(Direction, -Value);
		}
	}
}

void UIJ_PlayerMovement::Dash(FKey key)
{
	// 플레이어가 떠있는 상태, 공격, FinishMontage 상태가 아닐 때
	//(W,S 키는 가속 방향으로 회전, A,D 키는 회전 X), Player의 forwardVector 방향으로 이동, 플레이어의 AnimInstance에 접근해 몽타주 실행

	globalKey = key;

	if (me->playerFSM->anim->bCanDash == true)
	{
		// 사이드 뷰 시점이 아닐 땐
		if (me->bIsSideView == false)
			GeneralDash();
		else
			SideViewDash();
	}
	if (me->playerFSM->m_state == EPlayerState::Damaged)
	{
		Damaged_Off();
	}

	dashCurrentTime = 0.f;
}


void UIJ_PlayerMovement::GeneralDash()
{
	if (me->GetCharacterMovement()->IsFalling() == false && me->bIsBaseAttackPlaying == false && me->bIsDashPlaying == false && (bIsKillMontage == false))
	{
		if (dashCurrentTime >= dashDelayTime)
		{
			keyName = globalKey.GetFName();
			me->playerFSM->m_state = EPlayerState::Dash;
			me->bIsDashPlaying = true;


			// W 키를 눌렀다면
			if (keyName == "W")
			{
				// 만약 직정 회피상태조건에 맞았다면 직전회피함수를 사용
				if (me->bIsEvasion == true)
					Evasion(globalKey);
				else
				{
					me->GetCharacterMovement()->bOrientRotationToMovement = true;
					const FRotator Rotation = FRotator(0.f, me->Controller->GetControlRotation().Yaw, 0.f);
					me->SetActorRotation(Rotation);
					me->LaunchCharacter(FVector(me->GetActorForwardVector().X, me->GetActorForwardVector().Y, 0.f) * 3000, true, true);
					me->playerFSM->anim->DashForward();


					//me->playerFSM->anim->Montage_IsPlaying(dash)
				}
			}

			if (keyName == "S")
			{
				if (me->bIsEvasion == true)
					Evasion(globalKey);
				else
				{
					me->GetCharacterMovement()->bOrientRotationToMovement = true;
					const FRotator Rotation = FRotator(0.f, me->Controller->GetControlRotation().Yaw, 0.f);
					me->SetActorRotation(Rotation);
					me->LaunchCharacter(me->GetActorForwardVector() * 3000, true, true);
					me->playerFSM->anim->DashBack();
				}
			}

			if (keyName == "D")
			{
				if (me->bIsEvasion == true)
					Evasion(globalKey);
				else
				{
					me->GetCharacterMovement()->bOrientRotationToMovement = false;
					const FRotator Rotation = FRotator(0.f, me->Controller->GetControlRotation().Yaw, 0.f);
					me->SetActorRotation(Rotation);
					me->LaunchCharacter(me->GetActorRightVector() * 3000, true, true);
					me->playerFSM->anim->DashRight();
				}
			}

			if (keyName == "A")
			{
				if (me->bIsEvasion == true)
					Evasion(globalKey);
				else
				{
					me->GetCharacterMovement()->bOrientRotationToMovement = false;
					const FRotator Rotation = FRotator(0.f, me->Controller->GetControlRotation().Yaw, 0.f);
					me->SetActorRotation(Rotation);
					me->LaunchCharacter(me->GetActorRightVector() * -3000, true, true);
					me->playerFSM->anim->DashLeft();
				}

			}
		}
	}
}

void UIJ_PlayerMovement::SideViewDash()
{
	if (me->GetCharacterMovement()->IsFalling() == false && me->bIsBaseAttackPlaying == false && me->bIsDashPlaying == false && (bIsKillMontage == false))
	{
		if (dashCurrentTime >= dashDelayTime)
		{
			keyName = globalKey.GetFName();
			me->playerFSM->m_state = EPlayerState::Dash;

			if (keyName == "A")
			{
				if (me->bIsEvasion == true)
					Evasion(globalKey);
				else
				{
					me->GetCharacterMovement()->bOrientRotationToMovement = true;
					const FRotator Rotation = FRotator(0.f, me->GetMesh()->GetRelativeRotation().Yaw, 0.f);
					me->SetActorRotation(Rotation);
					me->LaunchCharacter(me->GetActorRightVector() * 3000, true, true);
					me->playerFSM->anim->DashForward();
				}
			}

			if (keyName == "D")
			{
				if (me->bIsEvasion == true)
					Evasion(globalKey);
				else
				{
					me->GetCharacterMovement()->bOrientRotationToMovement = true;
					const FRotator Rotation = FRotator(0.f, me->GetMesh()->GetRelativeRotation().Yaw, 0.f);
					me->SetActorRotation(Rotation);
					me->LaunchCharacter(me->GetActorRightVector() * -3000, true, true);
					me->playerFSM->anim->DashForward();
				}
			}
		}
	}
}

void UIJ_PlayerMovement::Damaged_Off()
{
	if (me->GetCharacterMovement()->IsFalling() == false && me->bIsBaseAttackPlaying == false && me->bIsDashPlaying == false && (bIsKillMontage == false))
	{

		keyName = globalKey.GetFName();
		me->playerFSM->m_state = EPlayerState::Idle;

		me->bIsDashPlaying = true;

		if (keyName == "W")
		{
			me->GetCharacterMovement()->bOrientRotationToMovement = true;
			const FRotator Rotation = FRotator(0.f, me->Controller->GetControlRotation().Yaw, 0.f);
			me->SetActorRotation(Rotation);
			me->LaunchCharacter(FVector(me->GetActorForwardVector().X, me->GetActorForwardVector().Y, 0.f) * 3000, true, true);
			me->playerFSM->anim->Damaged_Off();
		}

		if (keyName == "S")
		{
			me->GetCharacterMovement()->bOrientRotationToMovement = true;
			const FRotator Rotation = FRotator(0.f, me->Controller->GetControlRotation().Yaw, 0.f);
			me->SetActorRotation(Rotation);
			me->LaunchCharacter(me->GetActorForwardVector() * 3000, true, true);
			me->playerFSM->anim->Damaged_Off();
		}

		if (keyName == "D")
		{
			me->GetCharacterMovement()->bOrientRotationToMovement = false;
			const FRotator Rotation = FRotator(0.f, me->Controller->GetControlRotation().Yaw, 0.f);
			me->SetActorRotation(Rotation);
			me->LaunchCharacter(me->GetActorRightVector() * 3000, true, true);
			me->playerFSM->anim->Damaged_Off();
		}

		if (keyName == "A")
		{
			me->GetCharacterMovement()->bOrientRotationToMovement = false;
			const FRotator Rotation = FRotator(0.f, me->Controller->GetControlRotation().Yaw, 0.f);
			me->SetActorRotation(Rotation);
			me->LaunchCharacter(me->GetActorRightVector() * -3000, true, true);
			me->playerFSM->anim->Damaged_Off();
		}
	}
}

void UIJ_PlayerMovement::Attack()
{
	if (me->playerFSM->anim->bCanAttack == true)
	{
		// 대쉬, FinishMontage가 실행중이 아닐 때
		if ((me->bIsDashPlaying == false) && (bIsKillMontage == false))
		{
			me->bIsBaseAttackPlaying = true;

			if (me->playerFSM->m_state == EPlayerState::Idle || me->playerFSM->m_state == EPlayerState::Attack)
			{
				if (me->bIsBaseAttackMontagePlaying == false)
				{
					me->bIsBaseAttackMontagePlaying = true;

					// 공격 몽타주 실행
					me->PlayAnimMontage(me->baseAttackMontage);
				}
			}

			else if (me->playerFSM->m_state == EPlayerState::DashAttack || me->playerFSM->m_state == EPlayerState::Run)
			{
				if (me->bIsDashAttackMontagePlaying == false)
				{
					me->bIsDashAttackMontagePlaying = true;
					// 대쉬공격 몽타주 실행
					me->PlayAnimMontage(me->dashAttackMontage);
				}
			}
		}
	}
}

void UIJ_PlayerMovement::DefenceOn()
{
	if (me->playerFSM->m_state == EPlayerState::Idle || me->playerFSM->m_state == EPlayerState::Run && (bIsKillMontage == false))
	{
		// Defence 상태로 변경
		me->playerFSM->anim->bIsDefenceOn = true;
		me->playerFSM->m_state = EPlayerState::Defence;
	}
}

void UIJ_PlayerMovement::StopDefence()
{
	// Idle 상태로 변경
	me->playerFSM->anim->bIsDefenceOn = false;
	me->playerFSM->m_state = EPlayerState::Idle;
}

void UIJ_PlayerMovement::Evasion(FKey key)
{
	if ((me->bIsEvasion == true) && (bIsKillMontage == false))
	{
		// 이펙트를 가지고 있는 액터를 스폰
		auto spawnAvoid = GetWorld()->SpawnActor<AIJ_AvoidActor>(avoidActorFactory, me->GetActorTransform());
		// 월드시간 1/10
		GetWorld()->GetWorldSettings()->SetTimeDilation(0.1f);
		// 카메라릭 스피드 1/1000
		me->GetCameraBoom()->CameraLagSpeed = 0.001f;
		// 히든
		me->GetMesh()->SetHiddenInGame(true);
		// 무기 히든
		me->GetMesh()->GetChildComponent(0)->SetHiddenInGame(true);
		// 이펙트 활성화
		me->GetMesh()->GetChildComponent(1)->SetVisibility(true);
		// 가속방향 회전 false
		me->GetCharacterMovement()->bOrientRotationToMovement = false;
		const FRotator Rotation = FRotator(0.f, me->Controller->GetControlRotation().Yaw, 0.f);
		me->SetActorRotation(Rotation);

		if (me->bIsSideView == false)
		{
			if (key == "W")
			{
				me->LaunchCharacter(me->GetActorForwardVector() * 5000.f, true, true);
				me->playerFSM->anim->EvasionForward();
			}
			if (key == "S")
			{
				me->LaunchCharacter(me->GetActorForwardVector() * -5000.f, true, true);
				me->playerFSM->anim->EvasionBack();
			}
			if (key == "D")
			{
				me->LaunchCharacter(me->GetActorRightVector() * 3000.f, true, true);
				me->playerFSM->anim->EvasionRight();
			}
			if (key == "A")
			{
				me->LaunchCharacter(me->GetActorRightVector() * -3000.f, true, true);
				me->playerFSM->anim->EvasionLeft();
			}
		}

		// SideView 라면 키 변경
		if (me->bIsSideView == true)
		{
			if (key == "A")
			{
				me->LaunchCharacter(me->GetActorForwardVector() * 5000.f, true, true);
				me->playerFSM->anim->EvasionForward();
			}
			if (key == "D")
			{
				me->LaunchCharacter(me->GetActorForwardVector() * -5000.f, true, true);
				me->playerFSM->anim->EvasionBack();
			}
		}
		me->bIsTakeDamage = false;
		me->currentEvasionTime = 0.f;
	}
}

void UIJ_PlayerMovement::AndroidBotFire(float Value)
{
	fireCurrentTime += GetWorld()->DeltaTimeSeconds;

	// 게임모드 캐스팅
	auto gameMode = Cast<AMJ_ProjectGameMode>(GetWorld()->GetAuthGameMode());

	// 0.1 초에 한발
	if (Value > 0.f && fireCurrentTime >= fireDelayTime)
	{
		if (gameMode)
		{
			// 화염 총구 이펙트 실행
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), fireStartFX, me->androidBot->GetComponentLocation(), me->GetMesh()->GetComponentRotation());

			// 게임모드에서 총알 가져옴
			auto bullet = gameMode->GetBullet();
			// 총알 활성화
			gameMode->SetBulletActive(bullet, true);

			// 배치
			bullet->SetActorLocation(me->androidBot->GetComponentLocation());
			bullet->SetActorRotation(me->androidBot->GetComponentRotation());

			// CameraShake 
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(bulletCameraShake);
			// 0.1초를 지나면 0 으로 초기화
			fireCurrentTime = 0.f;
		}
	}
}

void UIJ_PlayerMovement::AndroidBotAttackOn()
{
	me->bIsAndroidAttack = true;
}

void UIJ_PlayerMovement::AndroidBotAttackOff()
{
	me->bIsAndroidAttack = false;

}

void UIJ_PlayerMovement::Jump()
{
	if (me->playerFSM->anim->bCanJump == true)
	{
		if (bIsKillMontage == false)
		{
			me->Jump();
			if (me->playerFSM->m_state != EPlayerState::Run)
			{
				me->playerFSM->m_state = EPlayerState::Jump;
			}
			else
			{
				me->playerFSM->m_state = EPlayerState::RunJump;
			}
			

			if (me->JumpCurrentCount == 1)
			{
				me->playerFSM->anim->DoubleJump();
			}
		}
	}
}

void UIJ_PlayerMovement::StopJumping()
{
	if (bIsKillMontage == false)
		me->StopJumping();
}

