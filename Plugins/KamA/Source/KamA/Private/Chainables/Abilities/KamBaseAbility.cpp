// Fill out your copyright notice in the Description page of Project Settings.

#include "Chainables/Abilities/KamBaseAbility.h"

#include "Chainables/Abilities/KamProjectileAbility.h"
#include "Chainables/Abilities/KamExplosionAbility.h"
#include "Chainables/Abilities/KamAreaAbility.h"
#include "Chainables/Abilities/KamTargetedAbility.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void AKamBaseAbility::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add("KamAbility");

	TArray<USceneComponent*> components;
	RootComponent->GetChildrenComponents(true, components);
	for (USceneComponent* component : components)
		componentsToScale.Add(FScalableComponent(component->GetComponentTransform().GetScale3D(), component));

	OnCreate();
}

TArray<UKamUnit*> AKamBaseAbility::GetUnitsToIgnore()
{
	return unitsToIgnore;
}

void AKamBaseAbility::SetSize(float newSize)
{
	Size = newSize;

	for (FScalableComponent component : componentsToScale)
	{
		if (!scaleBlockedComponents.Contains(component.Component))
		{
			component.Component->SetWorldScale3D(component.BaseSize * Size);
		}
	}
}

KamHitData AKamBaseAbility::CreateHitData()
{
	// Create Hit Data
	KamHitData HitData;
	HitData.Caster = casterSnapshot;
	HitData.Element = Element;
	HitData.GrossDamage = BaseDamage + casterSnapshot->GetAttribute(EKamAttribute::AT_Power) * PowerMultiplier;
	HitData.AbilityBaseCritChance = BaseCritChance;
	HitData.AbilityBaseCritBonus = BaseCritBonus;

	return HitData;
}

void AKamBaseAbility::DestroyAbility()
{
	if (markedToDestroy) return;

	// Call OnDestroy Event
	OnDestroy();

	// Destroy Ability
	markedToDestroy = true;

	// Deactivate Components
	for (USceneComponent* component : componentsToDeactivateOnDelayedDestroy)
	{
		component->Deactivate();
		component->SetActive(false);
		component->SetVisibility(false);
	}
}

void AKamBaseAbility::BlockComponentFromScaling(USceneComponent* component)
{
	if (!component) return;

	scaleBlockedComponents.Add(component);
}

void AKamBaseAbility::BlockComponentsFromScaling(TArray<USceneComponent*> components)
{
	for (USceneComponent* component : components)
	{
		BlockComponentFromScaling(component);
	}
}

void AKamBaseAbility::AddComponentToScale(USceneComponent* component)
{
	if (!component) return;

	componentsToScale.Add(FScalableComponent(component->GetComponentTransform().GetScale3D(), component));
}

void AKamBaseAbility::AddComponentsToScale(TArray<USceneComponent*> components)
{
	for (USceneComponent* component : components)
	{
		AddComponentToScale(component);
	}
}

void AKamBaseAbility::AddComponentToDeactivateOnDelayedDestroy(USceneComponent* component)
{
	if (!component) return;

	componentsToDeactivateOnDelayedDestroy.Add(component);
}

void AKamBaseAbility::AddComponentsToDeactivateOnDelayedDestroy(TArray<USceneComponent*> components)
{
	for (USceneComponent* component : components)
	{
		AddComponentToDeactivateOnDelayedDestroy(component);
	}
}

void AKamBaseAbility::SetUnitToIgnore(UKamUnit* unitToIgnore)
{
	if (!unitToIgnore) return;

	unitsToIgnore.Add(unitToIgnore);
}

void AKamBaseAbility::SetUnitsToIgnore(TArray<UKamUnit*> _unitsToIgnore)
{
	if (_unitsToIgnore.Num() == 0) return;

	for (UKamUnit* unit : _unitsToIgnore)
	{
		unitsToIgnore.Add(unit);
	}
}