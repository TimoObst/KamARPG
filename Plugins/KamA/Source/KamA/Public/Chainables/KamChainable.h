// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Units/KamUnitSnapshot.h"
#include "Units/KamUnit.h"
#include "Chainables/Abilities/KamTargetType.h"
#include "Chainables/KamHitData.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KamChainable.generated.h"


// Forward Declare //
class AKamBaseAbility;
class AKamProjectileAbility;
class AKamExplosionAbility;
class AKamAreaAbility;
class AKamTargetedAbility;


//! Actor to Deferred Spawn at end of frame
struct KAMA_API FDefferedChain
{
public:
	FDefferedChain(FTransform transform, AKamChainable* chain)
		: Transform(transform), Chain(chain) {};

	//! Transform to spawn at
	FTransform Transform;
	//! Object to spawn
	AKamChainable* Chain = nullptr;
};


//! Abstract Parent for Chainable Objects
/*!
	This class is inherited by anything that needs to be Chained by Abilities or Status Effects.
	It contains all of the Chaining functions.
*/
UCLASS(Abstract)
class KAMA_API AKamChainable : public AActor
{
public:
	// Constructor //
	AKamChainable(const FObjectInitializer& ObjectInitializer);

	GENERATED_BODY()

protected:
	//! Snapshot of the Chainable caster at cast
	UPROPERTY(BlueprintGetter = GetCasterSnapshot)
		UKamUnitSnapshot* casterSnapshot = nullptr;
	//! Pointer reference to the caster Unit
	UPROPERTY(BlueprintGetter = GetCasterUnit)
		UKamUnit* casterUnit = nullptr;

#ifndef DOXYGEN_SKIP
	// Post Physics Tick //
	FActorTickFunction LateTickFunction;
	bool isPostPhysics = false;

	// Actors To Spawn //
	TArray<FDefferedChain> chainsToSpawn;

	// Destroy Object Variables //
	bool markedToDestroy = false;
	bool delayDestroy = false;
	float delayDestroyBy = -1.0f;
	float delayDestroyTimer = 0.0f;

protected:
	// Update Functions //
	virtual void RegisterActorTickFunctions(bool bRegister) override;
	virtual void Update(float DeltaTime);
	virtual void LateUpdate(float DeltaTime);
public:
	virtual void Tick(float DeltaTime) override;

protected:
	// Destroy Object //
	virtual void ToDestroyHandler(float DeltaTime);

	// Deal Damage Functions //
	virtual void HitHandler(UKamUnit* target);
	virtual KamHitData CreateHitData();

public:
	// Setter Functions //
	void SetCasterSnapshot(UKamUnitSnapshot* caster);
	void SetCasterUnit(UKamUnit* unit);

	// Chain Ability //
	void PassCasterInChain(AKamChainable* chain);
#endif // DOXYGEN_SKIP

	//! Get the Snapshot of the original caster
	UFUNCTION(BlueprintGetter)
		UKamUnitSnapshot* GetCasterSnapshot();
	//! Get the Unit reference to the original caster
	UFUNCTION(BlueprintGetter)
		UKamUnit* GetCasterUnit();

	//! Chain Ability from Actor Location
	UFUNCTION(BlueprintCallable, Category = "Chain\|Ability")
		AKamBaseAbility* ChainAbility(TSubclassOf<AKamBaseAbility> toChain);
	//! Chain Ability from Unit Location
	UFUNCTION(BlueprintCallable, Category = "Chain\|Ability")
		AKamBaseAbility* ChainAbilityFromUnit(TSubclassOf<AKamBaseAbility> toChain, UKamUnit* unit);
	//! Chain Projectile from Actor Location with Overridable Variables
	UFUNCTION(BlueprintCallable, Category = "Chain\|Ability")
		AKamProjectileAbility* ChainProjectile(TSubclassOf<AKamProjectileAbility> toChain, float newSize, float newAngle, float newSpeed, float newMaxDistance);
	//! Chain Projectile from Unit Location with Overridable Variables
	UFUNCTION(BlueprintCallable, Category = "Chain\|Ability")
		AKamProjectileAbility* ChainProjectileFromUnit(TSubclassOf<AKamProjectileAbility> toChain, UKamUnit* unit, float newSize, float newAngle, float newSpeed, float newMaxDistance);
	//! Chain Explosion from Actor Location with Overridable Variables
	UFUNCTION(BlueprintCallable, Category = "Chain\|Ability")
		AKamExplosionAbility* ChainExplosion(TSubclassOf<AKamExplosionAbility> toChain, float startSize, float endSize, float lifeSpan);
	//! Chain Explosion from Unit Location with Overridable Variables
	UFUNCTION(BlueprintCallable, Category = "Chain\|Ability")
		AKamExplosionAbility* ChainExplosionFromUnit(TSubclassOf<AKamExplosionAbility> toChain, UKamUnit* unit, float startSize, float endSize, float lifeSpan);
	//! Chain Area from Actor Location with Overridable Variables
	UFUNCTION(BlueprintCallable, Category = "Chain\|Ability")
		AKamAreaAbility* ChainArea(TSubclassOf<AKamAreaAbility> toChain, float newSize, float newDuration, float newTickRate);
	//! Chain Area from Unit Location with Overridable Variables
	UFUNCTION(BlueprintCallable, Category = "Chain\|Ability")
		AKamAreaAbility* ChainAreaFromUnit(TSubclassOf<AKamAreaAbility> toChain, UKamUnit* unit, float newSize, float newDuration, float newTickRate);
	//! Chain Targeted from Actor Location with Overridable Variables
	UFUNCTION(BlueprintCallable, Category = "Chain\|Ability")
		AKamTargetedAbility* ChainTargeted(TSubclassOf<AKamTargetedAbility> toChain, EKamTargetType targetType, float newMaxRange, float newAngle);
	//! Chain Targeted from Unit Location with Overridable Variables
	UFUNCTION(BlueprintCallable, Category = "Chain\|Ability")
		AKamTargetedAbility* ChainTargetedFromUnit(TSubclassOf<AKamTargetedAbility> toChain, UKamUnit* fromUnit, EKamTargetType targetType, float newMaxRange, float newAngle);
	//! Chain Targeted with Preset Target Unit
	UFUNCTION(BlueprintCallable, Category = "Chain\|Ability")
		AKamTargetedAbility* ChainTargetedPreset(TSubclassOf<AKamTargetedAbility> toChain, UKamUnit* target);

	// Spawn Emitter //
	//! Spawn Emitter from Transform
	UFUNCTION(BlueprintCallable, Category = "Chain\|Emitter")
		void SpawnEmitterAtTransform(UParticleSystem* particle, FTransform transform);
	//! Spawn Emitter from Unit Transform
	UFUNCTION(BlueprintCallable, Category = "Chain\|Emitter")
		void SpawnEmitterAtUnit(UParticleSystem* particle, UKamUnit* unit);

	// Delay Destroy Functions //
	//! Delay Destroy of Ability indefinitely
	UFUNCTION(BlueprintCallable, Category = "Chain\|Destroy")
		void SetDelayDestroy(bool toDelay);
	//! Delay Destroy of Ability by seconds
	UFUNCTION(BlueprintCallable, Category = "Chain\|Destroy")
		void DelayDestroyBySeconds(float time);

	// Blueprint Implementable Calculations //
	//! Blueprint Implementable Function to calculate the crit chance 
	UFUNCTION(BlueprintImplementableEvent, Category = "Chain\|Calculations")
		float CritChanceCalculations();
	
	// Blueprint Implementable Events //
	//! Blueprint Event called when ability Crits
	UFUNCTION(BlueprintImplementableEvent, Category = "Kam Event")
		void OnCrit(UKamUnit* target);
	//! Blueprint Event called when ability Kills a Unit
	UFUNCTION(BlueprintImplementableEvent, Category = "Kam Event")
		void OnKill(UKamUnit* target);
	//! Blueprint Event called every frame
	UFUNCTION(BlueprintImplementableEvent, Category = "Kam Event")
		void OnUpdate(float deltaTime);
	//! Blueprint Event called when Chain is destroyed
	UFUNCTION(BlueprintImplementableEvent, Category = "Kam Event")
		void OnDestroy();
};
