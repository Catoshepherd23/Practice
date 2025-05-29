// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"


void UHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHealthBarWidget::UpdateHealth(float Current, float Max)
{
	if (HealthProgressBar && Max > 0.0f)
	{
		HealthProgressBar->SetPercent(Current / Max);
	}

	if (HealthText)
	{
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Current, Max)));
	}
}