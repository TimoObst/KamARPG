// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KamTargetType.generated.h"

UENUM(BlueprintType)
enum class EKamTargetType : uint8
{
	TT_Caster			UMETA(DisplayName = "Caster"),
	TT_RandomInRange	UMETA(DisplayName = "Random In Range"),
	TT_ClosestTarget	UMETA(DisplayName = "Closest Target"),
	TT_ClosestForward	UMETA(DisplayName = "Closest Forward"),
	TT_SetTarget		UMETA(DisplayName = "Preset Target"),
};