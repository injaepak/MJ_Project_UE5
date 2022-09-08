// Fill out your copyright notice in the Description page of Project Settings.


#include "IJ_PlayerFSM.h"
#include "IJ_Player.h"
#include "IJ_Player_AnimInstance.h"

// Sets default values for this component's properties
UIJ_PlayerFSM::UIJ_PlayerFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UIJ_PlayerFSM::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<AIJ_Player>(GetOwner());

	anim = Cast<UIJ_Player_AnimInstance>(me->GetMesh()->GetAnimInstance());

	anim->state = m_state;
}


// Called every frame
void UIJ_PlayerFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 플레이어 상태 뼈대
	switch (m_state)
	{
	case EPlayerState::Idle:
		IdleState();
		break;
	case EPlayerState::Move:
		MoveState();
		break;
	case EPlayerState::Run:
		RunState();
		break;
	case EPlayerState::Attack:
		AttackState();
		break;
	case EPlayerState::DashAttack:
		DashAttackState();
		break;
	case EPlayerState::Defence:
		DefenceState();
		break;
	case EPlayerState::Damaged:
		DamagedState();
		break;
	case EPlayerState::Die:
		DieState();
		break;
	}
}

void UIJ_PlayerFSM::IdleState()
{
	anim->state = m_state;
}

void UIJ_PlayerFSM::MoveState()
{
	anim->state = m_state;
}

void UIJ_PlayerFSM::RunState()
{
	anim->state = m_state;
}

void UIJ_PlayerFSM::DashState()
{
	anim->state = m_state;
}

void UIJ_PlayerFSM::DashAttackState()
{
	anim->state = m_state;
}

void UIJ_PlayerFSM::AttackState()
{
	anim->state = m_state;
}

void UIJ_PlayerFSM::DefenceState()
{
	anim->state = m_state;
}

void UIJ_PlayerFSM::DamagedState()
{
	anim->state = m_state;

}

void UIJ_PlayerFSM::DieState()
{
	anim->state = m_state;
}

