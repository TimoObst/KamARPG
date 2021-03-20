// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/KamUnit.h"

void UKamUnit::BeginPlay()
{
	// Apply Unique ID
	static int nextIndex = 0;
	index = nextIndex++;

	Super::BeginPlay();

	PrimaryComponentTick.bCanEverTick = true;

	owner = GetOwner();
	
	// set attributes
	attributes.Attributes = BaseAttributes.Attributes;
	health = attributes.GetAttribute(EKamAttribute::AT_Vitality);
}

void UKamUnit::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UKamUnit::TakeDamage(KamHitData& hitData)
{
	// Apply Crit
	hitData.AbilityCrit = DamageHasCrit(hitData.Caster->GetCritChance() + hitData.AbilityBaseCritChance, hitData.Caster, hitData.Element);
	if (hitData.AbilityCrit)
		hitData.GrossDamage = hitData.GrossDamage * (1.5f + ((hitData.Caster->GetAttribute(EKamAttribute::AT_CritBonus) + hitData.AbilityBaseCritBonus) / 100.0f));

	// Calculate Damage Reduction
	hitData.NetDamage = CalculateDamageReduction(hitData.GrossDamage, hitData.Caster, hitData.Element);
	hitData.DamageReduced = hitData.GrossDamage - hitData.NetDamage;

	// Apply Damage
	health -= hitData.NetDamage;

	// Call Damage Taken Event
	OnDamageTaken(hitData.NetDamage, hitData.Element, hitData.AbilityCrit);

	if (OnDamageTakenEvent.IsBound())
		OnDamageTakenEvent.Broadcast(hitData.NetDamage, hitData.Element, hitData.AbilityCrit);

	// Handle Death
	if (health <= 0)
	{
		hitData.TargetDied = true;
		OnDeath();
		
		if (OnDeathEvent.IsBound())
			OnDeathEvent.Broadcast();
	}
}

int UKamUnit::GetAttribute(EKamAttribute attribute)
{
	return attributes.GetAttribute(attribute);
}

int UKamUnit::GetBaseAttribute(EKamAttribute attribute)
{
	return BaseAttributes.GetAttribute(attribute);
}

UKamUnitSnapshot* UKamUnit::GetSnapShot()
{
	UKamUnitSnapshot* snapshot = NewObject<UKamUnitSnapshot>(UKamUnitSnapshot::StaticClass());

	float critchance = CalculateCritChance();
	snapshot->Initialize(attributes.Attributes, GetOwner()->GetTransform(), critchance);

	return snapshot;
}

void UKamUnit::SetHittable(bool toSet)
{
	isHittable = toSet;
}

bool UKamUnit::IsUnitHittable()
{
	return isHittable;
}

AActor* UKamUnit::GetUnitOwner()
{
	return owner;
}

FTransform UKamUnit::GetUnitTransform()
{
	return FTransform(GetOwner()->GetActorRotation(), GetOwner()->GetActorLocation());
}
