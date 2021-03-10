// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KamAttributes.generated.h"

UENUM(BlueprintType)
enum class EKamElement : uint8
{
	EL_True		UMETA(DisplayName = "Piercing"),
	EL_Physical	UMETA(DisplayName = "Physical"),
	EL_Magic	UMETA(DisplayName = "Magic"),
	EL_Fire		UMETA(DisplayName = "Fire"),
};

UENUM(BlueprintType)
enum class EKamAttribute : uint8
{
	AT_Level			UMETA(DisplayName = "Level"),
	AT_Power			UMETA(DisplayName = "Power"),
	AT_Vitality			UMETA(DisplayName = "Vitality"),
	AT_Health			UMETA(DisplayName = "Health"),
	AT_AttackSpeed		UMETA(DisplayName = "Attack Speed"),
	AT_CritChance		UMETA(DisplayName = "Crit Chance"),
	AT_CritBonus		UMETA(DisplayName = "Crit Bonus"),
	AT_Armor			UMETA(DisplayName = "Armor"),
	AT_MagicResist		UMETA(DisplayName = "Magic Resist"),
	AT_FireResist		UMETA(DisplayName = "Fire Resist"),
	AT_ResistAll		UMETA(DisplayName = "Resist All"),
	AT_MovementSpeed	UMETA(DisplayName = "Movement Speed"),
	AT_DefenseModifier	UMETA(DisplayName = "Defense Modifier"),
	AT_DamageModifier	UMETA(DisplayName = "Damage Modifier"),
};

USTRUCT(BlueprintType)
struct KAMARPG_API FKamAttributes
{
	GENERATED_BODY()

public:
	// Attributes
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EKamAttribute, int> Attributes = { { EKamAttribute::AT_Level, 1 },
											{ EKamAttribute::AT_Power, 10 },
											{ EKamAttribute::AT_Vitality, 100 },
											{ EKamAttribute::AT_AttackSpeed, 10 },
											{ EKamAttribute::AT_CritChance, 10 },
											{ EKamAttribute::AT_CritBonus, 10 },
											{ EKamAttribute::AT_Armor, 100 },
											{ EKamAttribute::AT_MagicResist, 10 },
											{ EKamAttribute::AT_ResistAll, 10 },
											{ EKamAttribute::AT_MovementSpeed, 100 } };

	int GetAttribute(EKamAttribute attribute);
};
