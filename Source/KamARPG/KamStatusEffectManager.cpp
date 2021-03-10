// Fill out your copyright notice in the Description page of Project Settings.

#include "KamStatusEffectManager.h"

#include "KamUnit.h"

FKamStatusEffectManager::FKamStatusEffectManager()
{
}

FKamStatusEffectManager::FKamStatusEffectManager(UKamUnit* owner)
{
	unit = owner;
}

FKamStatusEffectManager::~FKamStatusEffectManager()
{
}

void FKamStatusEffectManager::AddStatusEffect(TSubclassOf<UKamStatusEffectOld> statusEffect)
{
	// check if status effect already exists
		// reapply if so

	// apply if not already

	// Manipulate attributes if needed based on status effect
}

UKamStatusEffectOld* FKamStatusEffectManager::GetStatusEffectByName(FString name)
{
	
	return nullptr;
}
