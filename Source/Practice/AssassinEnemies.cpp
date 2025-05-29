// Fill out your copyright notice in the Description page of Project Settings.


#include "AssassinEnemies.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h" 
#include "GameFramework/CharacterMovementComponent.h"



AAssassinEnemies::AAssassinEnemies()
{
	PrimaryActorTick.bCanEverTick = true;
	
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UMyAttributeSet>(TEXT("AttributeSet"));
	
	
}

void AAssassinEnemies::BeginPlay()
{
	Super::BeginPlay();
	AttributeSet->InitMaxHealth(100.0f);
	AttributeSet->InitHealth(100.0f);

	if (AbilitySystemComp)
	{
		AbilitySystemComp->InitAbilityActorInfo(this, this);
	}
}

void AAssassinEnemies::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UAbilitySystemComponent* AAssassinEnemies::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}
float AAssassinEnemies::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsDead) return 0.0f; // If already dead, no damage is taken

	float CurrentHealth = AttributeSet->GetHealth();
	float NewHealth = CurrentHealth - DamageAmount;
	AttributeSet->SetHealth(NewHealth);
	UE_LOG(LogTemp, Warning, TEXT("Enemy Health: %f"), AttributeSet->GetHealth());
	if (NewHealth <= 0.0f)
	{
		bIsDead = true;	
		Death();		
	}

	return DamageAmount;
}

void AAssassinEnemies::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !AttackMontage)
	{
		return;
	}

	if (bIsAttacking || AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		return;
	}

	bIsAttacking = true;
}
void AAssassinEnemies::PlayDeathMontage()
{
	UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
	}
}

void AAssassinEnemies::AttackPlayer()
{
	if (!bIsAttacking)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Enemy attacks the player"));
		bIsAttacking = true;
		bIsEngagingPlayer = true;

		PlayAttackMontage();
		
		
	}

	if (!GetWorld()->GetTimerManager().IsTimerActive(AttackRepeatTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(AttackRepeatTimerHandle, this, &AAssassinEnemies::RepeatAttack, AttackRate, true);
	}

	
}

void AAssassinEnemies::RepeatAttack()
{
	float Distance = FVector::Dist(GetActorLocation(), UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation());
	
	if (Distance > 200.0f)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player out of range. Stopping attack."));

		bIsAttacking = false;
		bIsEngagingPlayer = false;
		bIsChasingPlayer = false; 

		GetWorld()->GetTimerManager().ClearTimer(AttackRepeatTimerHandle);
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Enemy attacks again"));
	PlayAnimMontage(AttackMontage);
	DealDamageToPlayer();
}

void AAssassinEnemies::DealDamageToPlayer()
{
	AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!Player) return;

	float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
	if (Distance <= AttackRange)
	{
		UGameplayStatics::ApplyDamage(Player, AttackDamage, GetController(), this, UDamageType::StaticClass());
		UE_LOG(LogTemp, Warning, TEXT("Enemy hit the player for %f damage"), AttackDamage);
	}
}

void AAssassinEnemies::StopAttack()
{
	//UE_LOG(LogTemp, Warning, TEXT("Stopping enemy attack"));
	bIsAttacking = false;
	bIsEngagingPlayer = false;
	GetWorld()->GetTimerManager().ClearTimer(AttackRepeatTimerHandle);
}

void AAssassinEnemies::Death()
{
	PlayDeathMontage();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	SetLifeSpan(5.0f);
}
