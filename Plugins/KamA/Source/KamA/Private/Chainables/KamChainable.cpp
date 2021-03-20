// Fill out your copyright notice in the Description page of Project Settings.

#include "Chainables/KamChainable.h"

#include "Chainables/Abilities/KamBaseAbility.h"
#include "Chainables/Abilities/KamProjectileAbility.h"
#include "Chainables/Abilities/KamExplosionAbility.h"
#include "Chainables/Abilities/KamAreaAbility.h"
#include "Chainables/Abilities/KamTargetedAbility.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AKamChainable::AKamChainable(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LateTickFunction.bCanEverTick = true;
	LateTickFunction.TickGroup = ETickingGroup::TG_PostPhysics;
}

// Register the tick functions
void AKamChainable::RegisterActorTickFunctions(bool bRegister)
{
	check(!IsTemplate());

	if (bRegister)
	{
		if (PrimaryActorTick.bCanEverTick)
		{
			PrimaryActorTick.Target = this;
			PrimaryActorTick.SetTickFunctionEnable(PrimaryActorTick.bStartWithTickEnabled || PrimaryActorTick.IsTickFunctionEnabled());
			PrimaryActorTick.RegisterTickFunction(GetLevel());
		}

		if (LateTickFunction.bCanEverTick)
		{
			LateTickFunction.Target = this;
			LateTickFunction.SetTickFunctionEnable(LateTickFunction.bStartWithTickEnabled || LateTickFunction.IsTickFunctionEnabled());
			LateTickFunction.RegisterTickFunction(GetLevel());
		}
	}
	else
	{
		if (PrimaryActorTick.IsTickFunctionRegistered())
		{
			PrimaryActorTick.UnRegisterTickFunction();
		}

		if (LateTickFunction.IsTickFunctionRegistered())
		{
			LateTickFunction.UnRegisterTickFunction();
		}
	}

	Super::RegisterActorTickFunctions(bRegister);
}

// Split the Tick function into Update and LateUpdate
void AKamChainable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isPostPhysics)
	{
		LateUpdate(DeltaTime);
		isPostPhysics = false;
	}
	else
	{
		if (!markedToDestroy)
		{
			Update(DeltaTime);
		}
		isPostPhysics = true;
	}
}

// PrePhysics Tick Group
void AKamChainable::Update(float DeltaTime)
{
	OnUpdate(DeltaTime);
}

// Post Physics Tick Group
void AKamChainable::LateUpdate(float DeltaTime)
{
	// Spawn Chained Actors
	if (chainsToSpawn.Num() > 0)
	{
		for (FDefferedChain toActivate : chainsToSpawn)
		{
			UGameplayStatics::FinishSpawningActor(toActivate.Chain, toActivate.Transform);
		}

		chainsToSpawn.Empty();
	}

	ToDestroyHandler(DeltaTime);
}

// Manage the Delay Destroy
void AKamChainable::ToDestroyHandler(float DeltaTime)
{
	if (!markedToDestroy) return;

	// Delay the Destroy() function
	if (delayDestroy)
	{
		// Delay by Time if Timer is Set
		if (delayDestroyBy > 0)
		{
			delayDestroyTimer += DeltaTime;

			if (delayDestroyTimer > delayDestroyBy)
			{
				Destroy();
			}
		}
	}
	else
	{
		Destroy();
	}
}

void AKamChainable::HitHandler(UKamUnit* target)
{
}

KamHitData AKamChainable::CreateHitData()
{
	KamHitData HitData;
	HitData.Caster = casterSnapshot;

	return HitData;
}

void AKamChainable::SetCasterSnapshot(UKamUnitSnapshot* caster)
{
	if (!caster) return;

	casterSnapshot = caster;
}

void AKamChainable::SetCasterUnit(UKamUnit* unit)
{
	if (!unit) return;

	casterUnit = unit;
}

UKamUnitSnapshot* AKamChainable::GetCasterSnapshot()
{
	return casterSnapshot;
}

UKamUnit* AKamChainable::GetCasterUnit()
{
	return casterUnit;
}

void AKamChainable::PassCasterInChain(AKamChainable* chain)
{
	chain->SetCasterSnapshot(casterSnapshot);
	chain->SetCasterUnit(casterUnit);
}

AKamBaseAbility* AKamChainable::ChainAbility(TSubclassOf<AKamBaseAbility> toChain)
{
	if (!toChain) return nullptr;

	FTransform spawnTransform(GetActorRotation(), GetActorLocation());
	AKamBaseAbility* newAbility = GetWorld()->SpawnActorDeferred<AKamBaseAbility>(toChain, spawnTransform);
	chainsToSpawn.Add(FDefferedChain(spawnTransform, newAbility));

	PassCasterInChain(newAbility);

	return newAbility;
}

AKamBaseAbility* AKamChainable::ChainAbilityFromUnit(TSubclassOf<AKamBaseAbility> toChain, UKamUnit* unit)
{
	if (!toChain || !unit) return nullptr;

	FTransform spawnTransform = unit->GetUnitTransform();
	AKamBaseAbility* newAbility = GetWorld()->SpawnActorDeferred<AKamBaseAbility>(toChain, spawnTransform);
	chainsToSpawn.Add(FDefferedChain(spawnTransform, newAbility));

	PassCasterInChain(newAbility);

	return newAbility;
}

AKamProjectileAbility* AKamChainable::ChainProjectile(TSubclassOf<AKamProjectileAbility> toChain, float newSize, float newAngle, float newSpeed, float newMaxDistance)
{
	if (!toChain) return nullptr;

	FTransform spawnTransform(GetActorRotation(), GetActorLocation());
	AKamProjectileAbility* projectile = GetWorld()->SpawnActorDeferred<AKamProjectileAbility>(toChain, spawnTransform);
	chainsToSpawn.Add(FDefferedChain(spawnTransform, projectile));

	projectile->OverrrideVariables(newSize, newAngle, newSpeed, newMaxDistance);
	PassCasterInChain(projectile);

	return projectile;
}

AKamProjectileAbility* AKamChainable::ChainProjectileFromUnit(TSubclassOf<AKamProjectileAbility> toChain, UKamUnit* unit, float newSize, float newAngle, float newSpeed, float newMaxDistance)
{
	if (!toChain || !unit) return nullptr;

	FTransform spawnTransform = unit->GetUnitTransform();
	AKamProjectileAbility* projectile = GetWorld()->SpawnActorDeferred<AKamProjectileAbility>(toChain, spawnTransform);
	chainsToSpawn.Add(FDefferedChain(spawnTransform, projectile));

	projectile->OverrrideVariables(newSize, newAngle, newSpeed, newMaxDistance);
	PassCasterInChain(projectile);

	return projectile;
}

AKamExplosionAbility* AKamChainable::ChainExplosion(TSubclassOf<AKamExplosionAbility> toChain, float startSize, float endSize, float lifeSpan)
{
	if (!toChain) return nullptr;

	FTransform spawnTransform(GetActorRotation(), GetActorLocation());
	AKamExplosionAbility* explosion = GetWorld()->SpawnActorDeferred<AKamExplosionAbility>(toChain, spawnTransform);
	chainsToSpawn.Add(FDefferedChain(spawnTransform, explosion));

	explosion->OverrideVariables(startSize, endSize, lifeSpan);
	PassCasterInChain(explosion);

	return explosion;
}

AKamExplosionAbility* AKamChainable::ChainExplosionFromUnit(TSubclassOf<AKamExplosionAbility> toChain, UKamUnit* unit, float startSize, float endSize, float lifeSpan)
{
	if (!toChain || !unit) return nullptr;

	FTransform spawnTransform = unit->GetUnitTransform();
	AKamExplosionAbility* explosion = GetWorld()->SpawnActorDeferred<AKamExplosionAbility>(toChain, spawnTransform);
	chainsToSpawn.Add(FDefferedChain(spawnTransform, explosion));

	explosion->OverrideVariables(startSize, endSize, lifeSpan);
	PassCasterInChain(explosion);

	return explosion;
}

AKamAreaAbility* AKamChainable::ChainArea(TSubclassOf<AKamAreaAbility> toChain, float newSize, float newDuration, float newTickRate)
{
	if (!toChain) return nullptr;

	FTransform spawnTransform(GetActorRotation(), GetActorLocation());
	AKamAreaAbility* area = GetWorld()->SpawnActorDeferred<AKamAreaAbility>(toChain, spawnTransform);
	chainsToSpawn.Add(FDefferedChain(spawnTransform, area));

	area->OverrideVariables(newSize, newDuration, newTickRate);
	PassCasterInChain(area);

	return area;
}

AKamAreaAbility* AKamChainable::ChainAreaFromUnit(TSubclassOf<AKamAreaAbility> toChain, UKamUnit* unit, float newSize, float newDuration, float newTickRate)
{
	if (!toChain || !unit) return nullptr;

	FTransform spawnTransform = unit->GetUnitTransform();
	AKamAreaAbility* area = GetWorld()->SpawnActorDeferred<AKamAreaAbility>(toChain, spawnTransform);
	chainsToSpawn.Add(FDefferedChain(spawnTransform, area));

	area->OverrideVariables(newSize, newDuration, newTickRate);
	PassCasterInChain(area);

	return area;
}

AKamTargetedAbility* AKamChainable::ChainTargeted(TSubclassOf<AKamTargetedAbility> toChain, EKamTargetType targetType, float newMaxRange, float newAngle)
{
	if (!toChain) return nullptr;

	FTransform spawnTransform(GetActorRotation(), GetActorLocation());
	AKamTargetedAbility* targeted = GetWorld()->SpawnActorDeferred<AKamTargetedAbility>(toChain, spawnTransform);
	chainsToSpawn.Add(FDefferedChain(spawnTransform, targeted));

	targeted->OverrideValues(targetType, newMaxRange, newAngle);
	PassCasterInChain(targeted);

	return targeted;
}

AKamTargetedAbility* AKamChainable::ChainTargetedFromUnit(TSubclassOf<AKamTargetedAbility> toChain, UKamUnit* fromUnit, EKamTargetType targetType, float newMaxRange, float newAngle)
{
	if (!toChain || !fromUnit) return nullptr;

	FTransform spawnTransform = fromUnit->GetUnitTransform();
	AKamTargetedAbility* targeted = GetWorld()->SpawnActorDeferred<AKamTargetedAbility>(toChain, spawnTransform);
	chainsToSpawn.Add(FDefferedChain(spawnTransform, targeted));

	targeted->OverrideValues(targetType, newMaxRange, newAngle);
	PassCasterInChain(targeted);

	return targeted;
}

AKamTargetedAbility* AKamChainable::ChainTargetedPreset(TSubclassOf<AKamTargetedAbility> toChain, UKamUnit* target)
{
	if (!toChain || !target) return nullptr;

	FTransform spawnTransform = target->GetUnitTransform();
	AKamTargetedAbility* targeted = GetWorld()->SpawnActorDeferred<AKamTargetedAbility>(toChain, spawnTransform);
	chainsToSpawn.Add(FDefferedChain(spawnTransform, targeted));

	targeted->OverrideValues(EKamTargetType::TT_SetTarget, 0, 0);
	targeted->SetTarget(target);
	PassCasterInChain(targeted);

	return targeted;
}

void AKamChainable::SpawnEmitterAtTransform(UParticleSystem* particle, FTransform transform)
{
	if (!particle) return;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particle, transform);
}

void AKamChainable::SpawnEmitterAtUnit(UParticleSystem* particle, UKamUnit* unit)
{
	if (!particle || !unit) return;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particle, unit->GetUnitTransform());
}

void AKamChainable::SetDelayDestroy(bool toDelay)
{
	delayDestroy = toDelay;
}

void AKamChainable::DelayDestroyBySeconds(float time)
{
	delayDestroy = true;
	delayDestroyBy = time;
}

