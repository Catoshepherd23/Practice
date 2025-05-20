// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AllEnemies.generated.h"

UCLASS()
class PRACTICE_API AAllEnemies : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAllEnemies();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bIsChasingPlayer = false;

	FTimerHandle RoamTimerHandle;

	UPROPERTY(EditAnywhere, Category = "AI")
	float RoamRadius = 500.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float RoamInterval = 3.0f;

	
	

	void Roam();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsChasingPlayer() const { return bIsChasingPlayer; }
	void SetIsChasingPlayer(bool bChasing) { bIsChasingPlayer = bChasing; }

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
