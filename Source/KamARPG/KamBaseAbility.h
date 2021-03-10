// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KamChainable.h"
#include "KamBaseAbility.generated.h"


/// Component to Scale ///
struct KAMARPG_API FScalableComponent
{
public:
	FScalableComponent(FVector baseSize, USceneComponent* component)
		: BaseSize(baseSize), Component(component) {};

	FVector BaseSize = FVector::OneVector;
	USceneComponent* Component = nullptr;
};


UCLASS(Abstract)
class KAMARPG_API AKamBaseAbility : public AKamChainable
{
	GENERATED_BODY()

protected:
	/// Ability Variables ///
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		FString Name = "New Ability";
	// Base Damage done by this Ability
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Core")
		float BaseDamage = 10.0f;
	// Increase Base Damage by Multiplying this with the Power Attribute of the Caster 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Core")
		float PowerMultiplier = 100.0f;
	// Element of the Damage 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Core")
		EKamElement Element = EKamElement::EL_True;
	// Scale the Ability Components by Size
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Core", BlueprintSetter = SetSize)
		float Size = 1;
	// Base Crit Chance of the Ability. Total Crit Chance = Caster Crit Chance + Ability Crit Chance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Attributes")
		float BaseCritChance = 0;
	// Base Crit Bonus of the Ability. Total Crit Bonus = Caster Crit Bonus + Ability Crit Bonus
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Attributes")
		float BaseCritBonus = 0;

	/// Units to Ignore ///
	UPROPERTY(BlueprintGetter = GetUnitsToIgnore)
		TArray<UKamUnit*> unitsToIgnore = TArray<UKamUnit*>();

	/// Scaling Components ///
	TArray<FScalableComponent> componentsToScale;
	TArray<USceneComponent*> scaleBlockedComponents;

	/// Deactivating On Destroy ///
	TArray<USceneComponent*> componentsToDeactivateOnDelayedDestroy;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// Deal Damage Functions ///
	virtual KamHitData CreateHitData() override;

public:
	/// Blueprint Getters ///
	UFUNCTION(BlueprintGetter, Category = "Ability")
		TArray<UKamUnit*> GetUnitsToIgnore();

	/// Blueprint Setters ///
	// Scale the Components of this Ability with Size
	UFUNCTION(BlueprintSetter, Category = "Ability")
		void SetSize(float newSize);

	/// Destroy Ability /// 
	// Destroy this Ability
	UFUNCTION(BlueprintCallable)
		void DestroyAbility();

	/// Manage Component Functions ///
	// Prevent a Component from scaling with the Size value
	UFUNCTION(BlueprintCallable, Category = "Ability\|Component")
		void BlockComponentFromScaling(USceneComponent* component);
	// Prevent Components from scaling with the Size value
	UFUNCTION(BlueprintCallable, Category = "Ability\|Component")
		void BlockComponentsFromScaling(TArray<USceneComponent*> components);
	// Add a Component to scale with the Size value
	UFUNCTION(BlueprintCallable, Category = "Ability\|Component")
		void AddComponentToScale(USceneComponent* component);
	// Add Components to scale with the Size value
	UFUNCTION(BlueprintCallable, Category = "Ability\|Component")
		void AddComponentsToScale(TArray<USceneComponent*> components);
	// Add a Component to be Deactivated and set Invisible when the Abilities Destroy is Delayed
	UFUNCTION(BlueprintCallable, Category = "Ability\|Component")
		void AddComponentToDeactivateOnDelayedDestroy(USceneComponent* component);
	// Add Components to be Deactivated and set Invisible when the Abilities Destroy is Delayed
	UFUNCTION(BlueprintCallable, Category = "Ability\|Component")
		void AddComponentsToDeactivateOnDelayedDestroy(TArray<USceneComponent*> components);

	/// Ignore Targets ///
	// Add a Unit to be ignored by this Ability
	UFUNCTION(BlueprintCallable, Category = "Ability\|Target")
		void SetUnitToIgnore(UKamUnit* unitToIgnore);
	// Add Units to be ignored by this Ability
	UFUNCTION(BlueprintCallable, Category = "Ability\|Target")
		void SetUnitsToIgnore(TArray<UKamUnit*> _unitsToIgnore);

	/// Blueprint Implementable Events ///
	// Event called when the Ability is Created (Begin Play)
	UFUNCTION(BlueprintImplementableEvent, Category = "Kam Event")
		void OnCreate();
	// Event called when the Ability Hits a Unit
	UFUNCTION(BlueprintImplementableEvent, Category = "Kam Event")
		void OnHit(UKamUnit* target);
	
};
