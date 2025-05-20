// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a CameraBoom to manage the camera distance and rotation
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;  // Follow distance behind player
	CameraBoom->bUsePawnControlRotation = true;  // Rotate arm based on controller

	// Create a FollowCamera attached to the CameraBoom
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;  // Camera does not rotate relative to the arm
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	StartRunning();
	StopRunning();
}

void AMyPlayer::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && !bIsAttacking)
	{
		// Get the controller's rotation and derive the forward direction
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		bIsMoving = true;
		// Move in that direction
		AddMovementInput(Direction, Value);
	}
	else
	{
		bIsMoving = false;		
	}
}

void AMyPlayer::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && !bIsAttacking)
	{
		// Get the controller's rotation and derive the right direction
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		bIsMoving = true;
		// Move in that direction
		AddMovementInput(Direction, Value);
	}
	else
	{
		bIsMoving = false;
	}
}
void AMyPlayer::StartRunning()
{	
	GetCharacterMovement()->MaxWalkSpeed = 600.0f; // Set to run speed
	bIsRunning = true;
}

void AMyPlayer::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.0f; // Set to walk speed
	bIsRunning = false;
}

void AMyPlayer::PlayJumpMontage()
{
	// Plays jump montage
	if (JumpMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(JumpMontage);
		
	}
}

void AMyPlayer::PlayAttackMontage()
{
	// Plays attack montage
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
			
	AnimInstance->Montage_Play(AttackMontage);

	if (!AnimInstance->OnMontageEnded.IsAlreadyBound(this, &AMyPlayer::OnAttackMontageEnded))
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyPlayer::OnAttackMontageEnded);
		
	}

	FTimerHandle TraceTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TraceTimerHandle, this, &AMyPlayer::PerformAttackTrace, 0.2f, false);
}

void AMyPlayer::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacking = false;

	// Remove any lingering delegates to avoid stacking them
	if (GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this, &AMyPlayer::OnAttackMontageEnded);
	}
}

void AMyPlayer::PerformAttackTrace()
{
	FVector Start = GetActorLocation();
	FVector Forward = GetActorForwardVector();
	FVector End = Start + (Forward * AttackRange);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	TArray<FHitResult> HitResults;

	// Perform a sphere trace
	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(AttackRadius), Params);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor != this)
			{
				// Apply damage to enemy
				UGameplayStatics::ApplyDamage(HitActor, AttackDamage, GetController(), this, UDamageType::StaticClass());
				UE_LOG(LogTemp, Warning, TEXT("Hit actor: %s"), *HitActor->GetName());
			}
		}
	}
	DrawDebugSphere(GetWorld(), End, AttackRadius, 12, FColor::Red, false, 1.0f); // Visualize the attack range
}

void AMyPlayer::PlayKickMontage()
{
	// Plays kick montage
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	if (bIsAttacking || AnimInstance->Montage_IsPlaying(KickMontage))
	{
		return;
	}
	
	if (!bIsMoving)
	{ 
		bIsAttacking = true;
		AnimInstance->Montage_Play(KickMontage);
	}

	if (!AnimInstance->OnMontageEnded.IsAlreadyBound(this, &AMyPlayer::OnAttackMontageEnded))
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyPlayer::OnAttackMontageEnded);
				
	}
}

void AMyPlayer::ModifyHealth(float Delta)
{
	
}

void AMyPlayer::Blink()
{
	// If the player cannot blink (ex on cooldown), exit the function
	if (!bCanBlink)
	{
		return;
	}
	// Get forward Direction
	FVector ForwardDirection = GetActorForwardVector();
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + (ForwardDirection * BlinkDistance);

	// Line trace to prevent blinking into walls
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
	
	// If obstacle was hit, creates a new destination just short of original
	if (bHit)
	{
		EndLocation = HitResult.Location - ForwardDirection * 50.0f;
	}

	// Spawns the visual effect for blink
	if (BlinkEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BlinkEffect, StartLocation, GetActorRotation());
	}

	// Instantly moves the player to the end location
	SetActorLocation(EndLocation, false);

	// Spawns the visual effect at the end of blink
	if (BlinkEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BlinkEffect, EndLocation, GetActorRotation());
	}

	//Cooldown Logic
	bCanBlink = false;
	GetWorld()->GetTimerManager().SetTimer(BlinkCooldownTimer, this, &AMyPlayer::ResetBlink, BlinkCooldown, false);
}

void AMyPlayer::ResetBlink()
{
	bCanBlink = true;
	GetWorld()->GetTimerManager().ClearTimer(BlinkCooldownTimer);
}
// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind axis for movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPlayer::MoveRight);

	// Bind action for jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyPlayer::PlayJumpMontage);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyPlayer::StopJumping);


	// Bind action for running
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyPlayer::StartRunning);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyPlayer::StopRunning);

	// Bind action for attack
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMyPlayer::PlayAttackMontage);

	// Bind action for kick
	PlayerInputComponent->BindAction("Kick", IE_Pressed, this, &AMyPlayer::PlayKickMontage);

	// Bind action for blink
	PlayerInputComponent->BindAction("Blink", IE_Pressed, this, &AMyPlayer::Blink);
	

	// Bind camera Rotation
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

}

