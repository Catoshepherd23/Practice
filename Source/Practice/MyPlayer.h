// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NiagaraSystem.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "MyAttributeSet.h" 
#include "AbilitySystemInterface.h"
#include "HealthBarWidget.h"

#include "MyPlayer.generated.h"

UCLASS()
class PRACTICE_API AMyPlayer : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayer();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Movement Functions
	void MoveForward(float Value);
	void MoveRight(float Value);

	
	
	void Blink();
	void ResetBlink();

	void PlayJumpMontage();
	void PlayAttackMontage();
	void PlayKickMontage();

	
	
	UPROPERTY()
	UAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY()
	UMyAttributeSet* AttributeSet;

	UPROPERTY()
	UUserWidget* PlayerHUD;

	void StartRunning();
	void StopRunning();

private:
	
	bool bIsRunning;
	bool bIsCrouching;
	bool bIsAttacking;
	bool bIsMoving;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void PerformAttackTrace();
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	
	// These properties are marked with UPROPERTY to allow them to be visible and editable in the Unreal Editor.
	// Camera Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* JumpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* AttackMontage2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* KickMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blink")
	float BlinkDistance = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blink")
	float BlinkCooldown = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Blink")
	UNiagaraSystem* BlinkEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRange = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackRadius = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackDamage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> HealthWidgetClass;

	UPROPERTY()
	UHealthBarWidget* HealthWidget;

	FTimerHandle BlinkCooldownTimer;
	bool bCanBlink = true;

	TSet<AActor*> HitActorsThisAttack;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	

};
