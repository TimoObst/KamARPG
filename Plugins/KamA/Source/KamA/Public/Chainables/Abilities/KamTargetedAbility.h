// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chainables/Abilities/KamBaseAbility.h"
#include "KamTargetedAbility.generated.h"

UCLASS(Abstract)
class KAMA_API AKamTargetedAbility : public AKamBaseAbility
{
	GENERATED_BODY()
	
protected:
	/// Blueprint Read Only ///
	// Unit Targeted by this Ability
	UPROPERTY(BlueprintGetter = GetTarget, BlueprintSetter = SetTarget)
		UKamUnit* TargetedUnit = nullptr;

	/// Blueprint Read Write ///
	// Targeting System used by this Ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Targeted")
		EKamTargetType TargetType = EKamTargetType::TT_Caster;
	// Maximum Range for Random In Range, Closest Target, Closest Forward 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Targeted")
		float MaxRange = 500.f;
	// Angle for Closest Forward
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Targeted")
		float Angle = 30.0f;

	/// Find Target Functions ///
	bool FindTarget();
	TArray<UKamUnit*> GetAllUnitsInRange();
	UKamUnit* GetRandomUnit();
	UKamUnit* GetClosestUnit();
	UKamUnit* GetClosestForwardUnit();

	/// Damage ///
	void HitHandler(UKamUnit* unitToHit);

	/// Implementable Blueprint Events ///
	// Event called when the Target is Found
	UFUNCTION(BlueprintImplementableEvent, Category = "Kam Event")
		void OnTargetFound(UKamUnit* target);
	// Event called when no Target is Found
	UFUNCTION(BlueprintImplementableEvent, Category = "Kam Event")
		void OnTargetNotFound();

public:
	void BeginPlay() override;
	void Update(float DeltaTime) override; // Targeted Ability has no Tick

	void OverrideValues(EKamTargetType newType, float newMaxRange, float newAngle);

	/// Blueprint Setters ///
	// Set the Target for PresetTarget
	UFUNCTION(BlueprintSetter)
		void SetTarget(UKamUnit* unit);

	/// Blueprint Getters ///
	UFUNCTION(BlueprintGetter)
		UKamUnit* GetTarget();
};
