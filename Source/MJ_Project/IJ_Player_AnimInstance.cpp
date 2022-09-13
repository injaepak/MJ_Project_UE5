// Fill out your copyright notice in the Description page of Project Settings.


#include "IJ_Player_AnimInstance.h"
#include "IJ_Player.h"
#include <Animation/AnimMontage.h>

// 상태머신의 움직임에 따른 애니메이션 몽타주 플레이 함수

void UIJ_Player_AnimInstance::Idle()
{
}

void UIJ_Player_AnimInstance::DashForward()
{
	Montage_Play(dashForwardMontage);
}

void UIJ_Player_AnimInstance::DashBack()
{
	Montage_Play(dashBackMontage);
}

void UIJ_Player_AnimInstance::DashLeft()
{
	Montage_Play(dashLeftMontage);
}

void UIJ_Player_AnimInstance::DashRight()
{
	Montage_Play(dashRightMontage);
}

void UIJ_Player_AnimInstance::Damaged_Off()
{
	Montage_Play(damaged_OffMontage);
}

void UIJ_Player_AnimInstance::Run()
{
}

void UIJ_Player_AnimInstance::BaseAttack()
{ 
	Montage_Play(baseAttackMontage);
}

void UIJ_Player_AnimInstance::DashAttack()
{
	Montage_Play(dashAttackMontage);
}

void UIJ_Player_AnimInstance::Damaged()
{
	Montage_Play(baseDamagedMontage);
}

void UIJ_Player_AnimInstance::DoubleJump()
{
	Montage_Play(doubleJumpMontage);
}

void UIJ_Player_AnimInstance::EvasionForward()
{
	Montage_Play(EvasionForwardMontage);
}

void UIJ_Player_AnimInstance::EvasionBack()
{
	Montage_Play(EvasionBackMontage);
}

void UIJ_Player_AnimInstance::EvasionLeft()
{
	Montage_Play(EvasionLeftMontage);
}

void UIJ_Player_AnimInstance::EvasionRight()
{
	Montage_Play(EvasionRightMontage);
}

void UIJ_Player_AnimInstance::kill()
{
	Montage_Play(killMontage);
}
