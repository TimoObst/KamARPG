// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Units/KamUnitSnapshot.h"
#include "Units/KamAttributes.h"

#include "CoreMinimal.h"

class KAMA_API KamHitData
{
public:
	UKamUnitSnapshot* Caster = nullptr;
	EKamElement Element = EKamElement::EL_True;

	float GrossDamage = 0.f;
	float DamageReduced = 0.f;
	float NetDamage = 0.f;

	float AbilityBaseCritChance = 0;
	float AbilityBaseCritBonus = 0;

	bool AbilityCrit = false;
	bool TargetDied = false;

	KamHitData();
	~KamHitData();
};
