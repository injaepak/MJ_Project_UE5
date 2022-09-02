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

	//SphereTrace ���� ����
	FVector StartLocation = MeshComp->GetSocketLocation(SocketName);
	FVector EndLocation = StartLocation +  ((MeshComp->GetOwner()->GetActorRotation().GetNormalized().Vector() + RotatorVector.Normalize()) * AttackDistance);

	//-----------------Ʈ���̽� �ɼ�----------------------------
	TArray<TEnumAsByte<EObjectTypeQuery>>types;
	types.Add(EObjectTypeQuery::ObjectTypeQuery3); //Pawn ���
	types.Add(EObjectTypeQuery::ObjectTypeQuery7); //Mosnter ���
	TArray<AActor*> ignores;
	TArray<FHitResult> hitResults;
	//----------------------------------------------------------


	//SphereTrace ������
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


	//Sphere Trace HitResultó��
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

					//���� ȣ��
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


