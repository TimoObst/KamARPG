// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KamStatusEffectManager.generated.h"

class UKamUnit;
class UKamStatusEffectOld;

USTRUCT(BlueprintType)
struct KAMARPG_API FKamStatusEffectManager
{
	GENERATED_BODY()

private:
	// Current time passed
	float gameTime = 0;
	// Owner of the statuseffectmanager
	UKamUnit* unit = nullptr;
	// Array of all currently applied status effects
	TArray<UKamStatusEffectOld*> statusEffects = TArray<UKamStatusEffectOld*>();

public:
	FKamStatusEffectManager();
	FKamStatusEffectManager(UKamUnit* owner);
	~FKamStatusEffectManager();

	void AddStatusEffect(TSubclassOf<UKamStatusEffectOld> statusEffect);

	UKamStatusEffectOld* GetStatusEffectByName(FString name);
};
