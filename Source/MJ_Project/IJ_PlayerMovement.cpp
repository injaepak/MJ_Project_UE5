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

	// ������ �� ����
	me->JumpMaxCount = 2;

	me->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void UIJ_PlayerMovement::InitializeComponent()
{
	me = Cast<AIJ_Player>(GetOwner());

	// ��������Ʈ�� �� �Լ��� ����ϰ� �ʹ�.
	// InitializeComponent �� BeginPlay ������ �ʱ�ȭ ������ش�.
	if (me)
	{
		me->OnInputDelegate.AddUObject(this, &UIJ_PlayerMovement::SetupPlayerInputComponent);
	}
}

// Called every frame
void UIJ_PlayerMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// �÷��̾ Run ������ ��
	if (me->playerFSM->m_state == EPlayerState::Run)
	{
		// Velocity �� 0 �̶��
		if (FVector(me->GetVelocity()).Size() == 0.f)
		{
			// Idle ���·� �����Ѵ�.
			me->playerFSM->m_state = EPlayerState::Idle;
		}
	}

	// ���� Defence ���¶�� MaxWalkSpeed = 200
	if (me->playerFSM->m_state == EPlayerState::Defence)
	{
		me->GetCharacterMovement()->MaxWalkSpeed = 200;
	}

	// ���� ȸ�ǻ��°� �ƴ϶��, �ʱ�ȭ�Ѵ�.
	if (me->bIsEvasion == false)
	{
		// ���� ���ο�, ī�Ŷ�, �Ž� Ű��, ����Ʈ ����
		GetWorld()->GetWorldSettings()->SetTimeDilation(1.f);
		me->GetCameraBoom()->CameraLagSpeed = 10.f;
		me->GetMesh()->SetHiddenInGame(false);
		me->GetMesh()->GetChildComponent(0)->SetHiddenInGame(false);
		me->GetMesh()->GetChildComponent(1)->SetVisibility(false);
	}

	dashCurrentTime += DeltaTime;
}

// �Է� �Լ� Ű ���ε�
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
	// ��Ʈ�ѷ��� Yaw �� ������ ȸ���ӵ�
	me->AddControllerYawInput(Rate * me->BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void UIJ_PlayerMovement::LookUpAtRate(float Rate)
{
	// ��Ʈ�ѷ��� Pitch �� ������ ȸ���ӵ�
	me->AddControllerPitchInput(Rate * me->BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void UIJ_PlayerMovement::Turn(float Rate)
{
	// Yaw �� ���� ȸ��
	me->AddControllerYawInput(Rate);
}

void UIJ_PlayerMovement::LookUp(float Rate)
{
	// Pitch �� ���� ȸ��
	me->AddControllerPitchInput(Rate);
}

void UIJ_PlayerMovement::MoveForward(float Value)
{

	// ��Ʈ�ѷ��� �ְ�, ���� 0 �� �ƴϰ�, FinishMontage �� ����ǰ� ���� ���� ��
	if ((me->Controller != nullptr) && (Value != 0.0f) && (bIsKillMontage == false))
	{
		// Run ���¶�� Speed 1000
		if (me->playerFSM->m_state == EPlayerState::Run)
		{
			me->GetCharacterMovement()->MaxWalkSpeed = 1000.f;
		}

		// Idle, Defence ���¶�� Speed 200
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
	// ��Ʈ�ѷ��� �ְ�, ���� 0 �� �ƴϰ�, FinishMontage �� ����ǰ� ���� ���� ��
	if ((me->Controller != nullptr) && (Value != 0.0f) && (bIsKillMontage == false))
	{
		// Run ���¶�� Speed 1000
		if (me->playerFSM->m_state == EPlayerState::Run)
		{
			me->GetCharacterMovement()->MaxWalkSpeed = 1000.f;
		}

		// Idle, Defence ���¶�� Speed 200
		else if (me->playerFSM->m_state == EPlayerState::Idle || me->playerFSM->m_state == EPlayerState::Defence)
		{
			me->GetCharacterMovement()->MaxWalkSpeed = 200;
		}

		// SideView �� �ƴ϶�� ��Ʈ�ѷ��� �����̼ǰ����� �����δ�.
		if (me->bIsSideView == false)
		{
			const FRotator Rotation = me->Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			me->AddMovementInput(Direction, Value);
		}
		// SideView ��� �Ž��� �����̼� ������ �����δ�.
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
	// �÷��̾ ���ִ� ����, ����, FinishMontage ���°� �ƴ� ��
	//(W,S Ű�� ���� �������� ȸ��, A,D Ű�� ȸ�� X), Player�� forwardVector �������� �̵�, �÷��̾��� AnimInstance�� ������ ��Ÿ�� ����

	globalKey = key;

	if (me->playerFSM->anim->bCanDash == true)
	{
		// ���̵� �� ������ �ƴ� ��
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


			// W Ű�� �����ٸ�
			if (keyName == "W")
			{
				// ���� ���� ȸ�ǻ������ǿ� �¾Ҵٸ� ����ȸ���Լ��� ���
				if (me->bIsEvasion == true)
					Evasion(globalKey);
				else
				{
					me->GetCharacterMovement()->bOrientRotationToMovement = true;
					const FRotator Rotation = FRotator(0.f, me->Controller->GetControlRotation().Yaw, 0.f);
					me->SetActorRotation(Rotation);
					me->LaunchCharacter(FVector(me->GetActorForwardVector().X, me->GetActorForwardVector().Y, 0.f) * 3000, true, true);
					me->playerFSM->anim->DashForward();
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
		// �뽬, FinishMontage�� �������� �ƴ� ��
		if ((me->bIsDashPlaying == false) && (bIsKillMontage == false))
		{
			me->bIsBaseAttackPlaying = true;

			if (me->playerFSM->m_state == EPlayerState::Idle || me->playerFSM->m_state == EPlayerState::Attack)
			{
				if (me->bIsBaseAttackMontagePlaying == false)
				{
					me->bIsBaseAttackMontagePlaying = true;

					// ���� ��Ÿ�� ����
					me->PlayAnimMontage(me->baseAttackMontage);
				}
			}

			else if (me->playerFSM->m_state == EPlayerState::DashAttack || me->playerFSM->m_state == EPlayerState::Run)
			{
				if (me->bIsDashAttackMontagePlaying == false)
				{
					me->bIsDashAttackMontagePlaying = true;
					// �뽬���� ��Ÿ�� ����
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
		// Defence ���·� ����
		me->playerFSM->anim->bIsDefenceOn = true;
		me->playerFSM->m_state = EPlayerState::Defence;
	}
}

void UIJ_PlayerMovement::StopDefence()
{
	// Idle ���·� ����
	me->playerFSM->anim->bIsDefenceOn = false;
	me->playerFSM->m_state = EPlayerState::Idle;
}

void UIJ_PlayerMovement::Evasion(FKey key)
{
	if ((me->bIsEvasion == true) && (bIsKillMontage == false))
	{
		// ����Ʈ�� ������ �ִ� ���͸� ����
		auto spawnAvoid = GetWorld()->SpawnActor<AIJ_AvoidActor>(avoidActorFactory, me->GetActorTransform());
		// ����ð� 1/10
		GetWorld()->GetWorldSettings()->SetTimeDilation(0.1f);
		// ī�޶� ���ǵ� 1/1000
		me->GetCameraBoom()->CameraLagSpeed = 0.001f;
		// ����
		me->GetMesh()->SetHiddenInGame(true);
		// ���� ����
		me->GetMesh()->GetChildComponent(0)->SetHiddenInGame(true);
		// ����Ʈ Ȱ��ȭ
		me->GetMesh()->GetChildComponent(1)->SetVisibility(true);
		// ���ӹ��� ȸ�� false
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

		// SideView ��� Ű ����
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

	// ���Ӹ�� ĳ����
	auto gameMode = Cast<AMJ_ProjectGameMode>(GetWorld()->GetAuthGameMode());

	// 0.1 �ʿ� �ѹ�
	if (Value > 0.f && fireCurrentTime >= fireDelayTime)
	{
		if (gameMode)
		{
			// ȭ�� �ѱ� ����Ʈ ����
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), fireStartFX, me->androidBot->GetComponentLocation(), me->GetMesh()->GetComponentRotation());

			// ���Ӹ�忡�� �Ѿ� ������
			auto bullet = gameMode->GetBullet();
			// �Ѿ� Ȱ��ȭ
			gameMode->SetBulletActive(bullet, true);

			// ��ġ
			bullet->SetActorLocation(me->androidBot->GetComponentLocation());
			bullet->SetActorRotation(me->androidBot->GetComponentRotation());

			// CameraShake 
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(bulletCameraShake);
			// 0.1�ʸ� ������ 0 ���� �ʱ�ȭ
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
	if (bIsKillMontage == false)
	{
		me->Jump();

		if (me->JumpCurrentCount == 1)
		{
			me->playerFSM->anim->DoubleJump();
		}
	}
}

void UIJ_PlayerMovement::StopJumping()
{
	if (bIsKillMontage == false)
		me->StopJumping();
}

