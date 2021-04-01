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

static class KAMA_API KamElements
{
public:
	KamElements();
	~KamElements();

	static FString GetElementEnumAsString(EElements element);
};
