// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KamUnitSnapshot.h"
#include "KamAttributes.h"

#include "CoreMinimal.h"

class KAMARPG_API KamHitData
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
