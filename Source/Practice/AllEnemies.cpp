// Fill out your copyright notice in the Description page of Project Settings.


#include "AllEnemies.h"
#include <NavigationSystem.h>
#include "NavigationPath.h"
#include "AIController.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAllEnemies::AAllEnemies()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAllEnemies::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(RoamTimerHandle, this, &AAllEnemies::Roam, RoamInterval, true);
	
}

void AAllEnemies::Roam()
{
	FVector Origin = GetActorLocation();
	FNavLocation RandomPoint;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	if (bIsChasingPlayer)
	{
		return;
	}

	

	if (NavSystem && NavSystem->GetRandomPointInNavigableRadius(Origin, RoamRadius, RandomPoint))
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			AIController->MoveToLocation(RandomPoint.Location);
		}
	}

	
	
}

// Called every frame
void AAllEnemies::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAllEnemies::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

