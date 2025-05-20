// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"

#include "EnemyAIController.generated.h"

/**
 * 
 */

class AAssassinEnemies;
UCLASS()
class PRACTICE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

protected:
	//virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void TryInitialize();
	virtual void OnPossess(APawn* InPawn) override;

	
	
	
	void OnMoveCompletedHandler(FAIRequestID RequestID, const FPathFollowingResult& Result);

public:
	void MoveToPlayer();

private:
	UPROPERTY()
	APawn* PlayerPawn;

	UPROPERTY()
	class AAssassinEnemies* ControlledEnemy;

	FTimerHandle DistanceCheckTimerHandle;
	FTimerHandle RechaseTimerHandle;
};
