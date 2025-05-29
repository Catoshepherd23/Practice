// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AllEnemies.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "MyAttributeSet.h"
#include "AssassinEnemies.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API AAssassinEnemies : public AAllEnemies, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AAssassinEnemies();
	virtual void Tick(float DeltaTime) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	void AttackPlayer();
	void Death();
	void RepeatAttack();
	void StopAttack();

	void DealDamageToPlayer();
	bool IsEngaging() const { return bIsEngagingPlayer; }

	FTimerHandle AttackRepeatTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackRange = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackRadius = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackRate = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* DeathMontage;

protected:
	virtual void BeginPlay() override;
	void PlayDeathMontage();
	void PlayAttackMontage();
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY()
	UMyAttributeSet* AttributeSet;
	
private:
	bool bIsDead = false;
	bool bIsAttacking;
	bool bIsEngagingPlayer;
};
