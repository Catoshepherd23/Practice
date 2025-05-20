// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "AssassinEnemies.h"
#include "AllEnemies.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "AIController.h"

AEnemyAIController::AEnemyAIController()
{
	
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	
	GetWorld()->GetTimerManager().SetTimer(DistanceCheckTimerHandle, this, &AEnemyAIController::TryInitialize, 1.0f, true);

}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	TryInitialize();
	GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AEnemyAIController::OnMoveCompletedHandler);
}
void AEnemyAIController::TryInitialize()
{

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	ControlledEnemy = Cast<AAssassinEnemies>(GetPawn());

	if (!PlayerPawn || !ControlledEnemy)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerPawn or ControlledEnemy is null. Retrying..."));
		
		return;
	}

	if (ControlledEnemy->IsEngaging())
	{
		UE_LOG(LogTemp, Warning, TEXT("Already engaging player. Skipping chase."));
		return;
	}

	float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), ControlledEnemy->GetActorLocation());
	//UE_LOG(LogTemp, Warning, TEXT("Distance to player: %f"), Distance);
	if (Distance > 160.0f && Distance <= 1000.0f)
	{
		if (!ControlledEnemy->IsChasingPlayer())
		{
			UE_LOG(LogTemp, Warning, TEXT("Player is nearby, moving to them."));
			ControlledEnemy->SetIsChasingPlayer(true);
			MoveToPlayer();
		}
	}
	else if (Distance <= 160.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is within attack range."));
		ControlledEnemy->AttackPlayer();  // This should handle isAttacking checks internally
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player too far, not moving."));
		ControlledEnemy->SetIsChasingPlayer(false);
	}
	
	
}

void AEnemyAIController::MoveToPlayer()
{
	if (PlayerPawn && ControlledEnemy)
	{
		
		
		if (ControlledEnemy->IsEngaging())
		{
			UE_LOG(LogTemp, Warning, TEXT("Already attacking.  Skipping MoveToPlayer"));
			return;
		}
		
		EPathFollowingRequestResult::Type MoveResult = MoveToActor(PlayerPawn, 50.0f);
		UE_LOG(LogTemp, Warning, TEXT("MoveToActor called on %s, result: %d"), *PlayerPawn->GetName(), static_cast<int32>(MoveResult));

		switch (MoveResult)
		{

		case EPathFollowingRequestResult::AlreadyAtGoal:

			UE_LOG(LogTemp, Warning, TEXT("AI already at goal.  No movement needed."));
			ControlledEnemy->AttackPlayer();
			break;

		case EPathFollowingRequestResult::RequestSuccessful:
			UE_LOG(LogTemp, Warning, TEXT("Move request sent successfully"));
			break;

		case EPathFollowingRequestResult::Failed:

			UE_LOG(LogTemp, Error, TEXT("MoveToActor failed."));
			break;

		default:
			UE_LOG(LogTemp, Warning, TEXT("Unknown move result"));
			break;
		}
	}
	
	
}

void AEnemyAIController::OnMoveCompletedHandler(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if (Result.Code != EPathFollowingResult::Success || !ControlledEnemy || !PlayerPawn)
	{
		return;
	}
	const float Distance = FVector::Dist(ControlledEnemy->GetActorLocation(), PlayerPawn->GetActorLocation());

	if (Distance <= 200.0f)
	{
		
		ControlledEnemy->AttackPlayer();
		
	}
		
	else
	{
		ControlledEnemy->StopAttack();
		MoveToPlayer();
	}
		
	
}