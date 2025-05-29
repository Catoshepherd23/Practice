// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "HealthBarWidget.generated.h"

/**
 * 
 */


UCLASS()
class PRACTICE_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateHealth(float Current, float Max);

protected:
	virtual void NativeConstruct() override;

	// Bind to progress bar and text from the widget blueprint
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* HealthText;
};
