// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "YS_MonsterBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpChanged);

UCLASS()
class MJ_PROJECT_API AYS_MonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AYS_MonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



private:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Combat, meta = (AllowPrivateAccess = "true"))
		float currentHealth = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Combat, meta = (AllowPrivateAccess = "true"))
		float maxHealth = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Combat, meta = (AllowPrivateAccess = "true"))
		bool bCanMontageAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Combat, meta = (AllowPrivateAccess = "true"))
		bool Die = false;


	UPROPERTY(EditAnywhere, Category = Combat, meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* behaviorTree;

	UPROPERTY(EditAnywhere, Category = Combat, meta = (AllowPrivateAccess = "true"))
		TArray<UAnimMontage*> basicAttackMontage;

	
	
	UPROPERTY(EditAnywhere, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* DamageMontage;

	UPROPERTY(EditAnywhere, Category = Combat, meta = (AllowPrivateAccess = "true"))
		UAnimMontage* dieMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Combat, meta = (AllowPrivateAccess = "true"))
		float knockBackDintance = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Combat, meta = (AllowPrivateAccess = "true"))
		FVector directionCal = {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Combat, meta = (AllowPrivateAccess = "true"))
		FVector FinalKnockBackLocation = {};


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = Combat, meta = (AllowPrivateAccess = "true"))
		bool bIsBoss = false;

	//기본공격 몽타주 불러오기
	UAnimMontage* GetBasicAttackMontage();


	//몬스터 체력 불러오기
	FORCEINLINE float GetMaxHealth() const { return maxHealth; }
	FORCEINLINE float GetCurrentHealth() const { return currentHealth; }
	FORCEINLINE float GetHpRatio() { return currentHealth / maxHealth; }

	//비헤이비어트리 불러오기
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return behaviorTree; }


	//몬스 체력 Set
	FORCEINLINE void SetMaxHealth(float Health) { maxHealth = Health; }
	FORCEINLINE void SetCurrentHealth(float Health) { currentHealth = Health; }
	void SetDamage(float Damage);


	//공격구현
	void Attack(UAnimMontage* AttackMontage);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	void Death();


	void KnockBack(AActor* Player);
	void KnockBackUpdate(float delta);

	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* lockOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AIJ_Player* player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UIJ_Player_AnimInstance* playerAnim;


	///HP
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HpBar;

	FOnHpChanged OnHpChanged;
};
