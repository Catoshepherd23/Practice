// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_DealDamage.h"
#include "MyPlayer.h"
#include "GameFramework/Actor.h"

void UAnimNotify_DealDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();

	if (Owner)
	{
		AMyPlayer* Player = Cast<AMyPlayer>(Owner);
		if (Player)
		{
			Player->PerformAttackTrace();
		}
	}
}

