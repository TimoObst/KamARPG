// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KamAttributes.h"
#include "KamUnitSnapshot.h"
#include "KamHitData.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KamUnit.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamageTakenSignature, float, damageTaken, EKamElement, damageElement, bool, isCrit);

//! Actor Component for combat interactive Units
/*!
	Component that handles interaction with Abilties and Status Effects.
	Manages Health and Attributes of Unit.
*/

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class KAMARPG_API UKamUnit : public UActorComponent
{
	GENERATED_BODY()

protected:
	//! Unique ID per Unit
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		int index = 0;

	//! Current health of the Unit
	UPROPERTY(VisibleAnywhere)
		int health = 1; 
	//! Owner of Unit Component
	UPROPERTY(BlueprintGetter = GetUnitOwner)
		AActor* owner = nullptr;
	//! Can interact with Abilities
	UPROPERTY(EditAnywhere, BlueprintSetter = SetHittable, BlueprintGetter = IsUnitHittable)
		bool isHittable = true;

	//! Current state of the Attributes
	FKamAttributes attributes = FKamAttributes();
	//! Base state of the Attributes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		FKamAttributes BaseAttributes = FKamAttributes();

#ifndef DOXYGEN_SKIP
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Add damage to the Unit
	void TakeDamage(KamHitData& hitData);
#endif // DOXYGEN_SKIP
	
	// Delegates //
	//! Delegate for Death Event
	UPROPERTY(BlueprintAssignable, Category = "Unit Event Dispatcher")
		FOnDeathSignature OnDeathEvent;
	//! Delegate for Damage Taken Event
	UPROPERTY(BlueprintAssignable, Category = "Unit Event Dispatcher")
		FOnDamageTakenSignature OnDamageTakenEvent;

	// Implementable Functions //
	//! Blueprint Implementable Function to calculate Damage Reduction
	UFUNCTION(BlueprintImplementableEvent)
		float CalculateDamageReduction(float damageIn, UKamUnitSnapshot* caster, EKamElement damageType);
	//! Blueprint Implementable Function to calculate Crit Chance
	UFUNCTION(BlueprintImplementableEvent)
		float CalculateCritChance();
	//! Blueprint Implementable Function to calculate if Damage has Crit
	UFUNCTION(BlueprintImplementableEvent)
		bool DamageHasCrit(float critChance, UKamUnitSnapshot* caster, EKamElement damageType);

	//! Blueprint Event called when Unit dies
	UFUNCTION(BlueprintImplementableEvent)
		void OnDeath();
	//! Blueprint Event called when Unit takes damage
	/*! This Event is skipped when damage taken is 0 */
	UFUNCTION(BlueprintImplementableEvent)
		void OnDamageTaken(const float DamageTaken, const EKamElement damageType, const bool isCriticalHit);

	// Callable Functions //
	//! Get the current value of an Attribute from Unit
	UFUNCTION(BlueprintCallable, Category = "Attributes")
		int GetAttribute(EKamAttribute attribute);
	//! Get the base value of an Attribute from Unit
	UFUNCTION(BlueprintCallable, Category = "Attributes")
		int GetBaseAttribute(EKamAttribute attribute);

	//! Get the Transform of this Unit
	UFUNCTION(BlueprintCallable, Category = "Unit")
		FTransform GetUnitTransform();
	//! Get a Snapshot of this Unit
	UFUNCTION(BlueprintCallable, Category = "Unit")
		UKamUnitSnapshot* GetSnapShot();

	// Setter Function //
	//! Set the isHittable bool
	UFUNCTION(BlueprintSetter, Category = "Unit")
		void SetHittable(bool toSet);

	// Getter Function //
	//! Get owning Actor of Unit
	UFUNCTION(BlueprintGetter, Category = "Unit")
		AActor* GetUnitOwner();
	//! Get isHittable bool
	UFUNCTION(BlueprintGetter, Category = "Unit")
		bool IsUnitHittable();
};