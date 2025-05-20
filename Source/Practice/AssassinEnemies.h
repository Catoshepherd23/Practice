// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AllEnemies.h"
#include "AssassinEnemies.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API AAssassinEnemies : public AAllEnemies
{
	GENERATED_BODY()
	
public:
	AAssassinEnemies();
	virtual void Tick(float DeltaTime) override;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	void AttackPlayer();
	void Death();
	void RepeatAttack();
	void StopAttack();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health = 100.0f;
	
private:
	bool bIsDead = false;
	bool bIsAttacking;
	bool bIsEngagingPlayer;
};
