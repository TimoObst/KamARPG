// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Units/KamAttributes.h"

#include "CoreMinimal.h"
#include "KamModifier.generated.h"

UENUM(BlueprintType)
enum class EKamModifierType : uint8
{
	MT_Damage		UMETA(DisplayName = "Damage"),
	MT_Heal			UMETA(DisplayName = "Heal"),
	MT_Attribute	UMETA(DisplayName = "Attribute")
};

UENUM(BlueprintType)
enum class EKamOperator : uint8
{
	OP_Additive		UMETA(DisplayName = "Flat"),
	OP_Percentage	UMETA(DisplayName = "Percentage")
};

USTRUCT(BlueprintType)
struct KAMA_API FKamModifier
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EKamAttribute Attribute = EKamAttribute::AT_Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EKamOperator Operator = EKamOperator::OP_Additive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Amount = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EKamElement Element = EKamElement::EL_True;

	FKamModifier();
	~FKamModifier();
};
