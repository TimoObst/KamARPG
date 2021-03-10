// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

UENUM(BlueprintType)
enum class EElements : uint8
{
	EL_TRUE		UMETA(DisplayName = "True"),
	EL_PHYSICAL	UMETA(DisplayName = "Physical"),
	EL_FIRE		UMETA(DisplayName = "Fire"),
	EL_MAGIC	UMETA(DisplayName = "Magic"),
	EL_POISON	UMETA(DisplayName = "Poison"),
	EL_SHADOW	UMETA(DisplayName = "Shadow")
};

static class KAMARPG_API KamARPG_Elements
{
public:
	KamARPG_Elements();
	~KamARPG_Elements();

	static FString GetElementEnumAsString(EElements element);
};
