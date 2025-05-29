// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MyAttributeSet.generated.h"

UCLASS()
class PRACTICE_API UMyAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UMyAttributeSet();

    /** Health Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData Health;

    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData MaxHealth;

    FORCEINLINE FGameplayAttribute GetHealthAttribute() const;
    FORCEINLINE float GetMaxHealth() const { return MaxHealth.GetCurrentValue(); }
    FORCEINLINE float GetHealth() const;
    FORCEINLINE void SetHealth(float NewValue);
    FORCEINLINE void InitHealth(float NewValue);
    FORCEINLINE void SetMaxHealth(float NewValue) { MaxHealth.SetCurrentValue(NewValue); }
    FORCEINLINE void InitMaxHealth(float NewValue) { MaxHealth.SetBaseValue(NewValue); MaxHealth.SetCurrentValue(NewValue); }
};

FORCEINLINE FGameplayAttribute UMyAttributeSet::GetHealthAttribute() const
{
    return FGameplayAttribute(FindFieldChecked<FProperty>(UMyAttributeSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UMyAttributeSet, Health)));
}

FORCEINLINE float UMyAttributeSet::GetHealth() const
{
    return Health.GetCurrentValue();
}

FORCEINLINE void UMyAttributeSet::SetHealth(float NewValue)
{
    float ClampedValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
    Health.SetCurrentValue(ClampedValue);
}

FORCEINLINE void UMyAttributeSet::InitHealth(float NewValue)
{
    float ClampedValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
    Health.SetBaseValue(ClampedValue);
    Health.SetCurrentValue(ClampedValue);
}
