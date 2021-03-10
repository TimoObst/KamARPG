// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KamAttributes.h"

#include "CoreMinimal.h"
#include "KamUnitSnapshot.generated.h"

UCLASS(BlueprintType)
class KAMARPG_API UKamUnitSnapshot : public UObject
{
	GENERATED_BODY()

private:
	TMap<EKamAttribute, int> attributes;
	FTransform transform;
	float critChance = 0.f;

public:
	UKamUnitSnapshot();
	~UKamUnitSnapshot();

	void Initialize(TMap<EKamAttribute, int> Attributes, FTransform Transform, float CritChance);

	UFUNCTION(BlueprintPure)
		int GetAttribute(EKamAttribute attribute);
	UFUNCTION(BlueprintPure)
		FVector GetPosition();
	UFUNCTION(BlueprintPure)
		FQuat GetRotation();
	UFUNCTION(BlueprintPure)
		float GetCritChance();
};
