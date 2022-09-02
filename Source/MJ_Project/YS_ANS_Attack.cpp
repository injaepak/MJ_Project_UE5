// Fill out your copyright notice in the Description page of Project Settings.


#include "YS_ANS_Attack.h"
#include "YS_MonsterBase.h"
#include "IJ_Player.h"
#include "Kismet/GameplayStatics.h"


void UYS_ANS_Attack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	DamagedPawn.Add(nullptr);
}

void UYS_ANS_Attack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{

	//SphereTrace 방향 세팅
	FVector StartLocation = MeshComp->GetSocketLocation(SocketName);
	FVector EndLocation = StartLocation +  ((MeshComp->GetOwner()->GetActorRotation().GetNormalized().Vector() + RotatorVector.Normalize()) * AttackDistance);

	//-----------------트레이스 옵션----------------------------
	TArray<TEnumAsByte<EObjectTypeQuery>>types;
	types.Add(EObjectTypeQuery::ObjectTypeQuery3); //Pawn 기반
	types.Add(EObjectTypeQuery::ObjectTypeQuery7); //Mosnter 기반
	TArray<AActor*> ignores;
	TArray<FHitResult> hitResults;
	//----------------------------------------------------------


	//SphereTrace 구현부
	bool Trace = UKismetSystemLibrary::SphereTraceMultiForObjects
	(MeshComp,
		StartLocation,
		EndLocation,
		AttackRadius,
		types,
		false,
		ignores,
		DebugType,
		hitResults,
		true,
		FLinearColor::Red,
		FLinearColor::Green, 3.f);


	//Sphere Trace HitResult처리
	if (Trace == true)
	{
		for (const FHitResult& hitResult : hitResults)
		{


			APawn* tracePawn = Cast<APawn>(hitResult.GetActor());
			TargetPawn.AddUnique(tracePawn);


		}
		if (TargetPawn[0] != nullptr)
		{
			for (int32 i = 0; i < TargetPawn.Num(); i++)
			{
				
				if (!DamagedPawn.IsValidIndex(0))
				{

					//공격 호출
					FDamageEvent Event(UDamageType::StaticClass());
					TargetPawn[i]->TakeDamage(AttackDamage, Event, MeshComp->GetOwner()->GetInstigatorController(), MeshComp->GetOwner());


					DamagedPawn.AddUnique(TargetPawn[i]);
				}
				else
				{
					for (int j = 0; j < DamagedPawn.Num(); j++)
					{
						if (DamagedPawn[j] == TargetPawn[i])
						{
							bCanAttack = false;
						}
					}
					
					if (bCanAttack == true)
					{

						
							FDamageEvent Event(UDamageType::StaticClass());
							TargetPawn[i]->TakeDamage(AttackDamage, Event, MeshComp->GetOwner()->GetInstigatorController(), MeshComp->GetOwner());

							DamagedPawn.AddUnique(TargetPawn[i]);

					}
					bCanAttack = true;

				}
						
				
				

			}
		}
		

		
		
	}
	


}

void UYS_ANS_Attack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	DamagedPawn.Empty();
	TargetPawn.Empty();
}


