// Fill out your copyright notice in the Description page of Project Settings.


#include "KamTargetedAbility.h"

#include "Engine/World.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/KismetSystemLibrary.h"

#include <Runtime/Engine/Classes/Engine/Engine.h>

void AKamTargetedAbility::BeginPlay()
{
	bool targetFound = FindTarget();

	Super::BeginPlay();

	if (targetFound)
	{
		OnTargetFound(TargetedUnit);
		HitHandler(TargetedUnit);
	}
	else
	{
		OnTargetNotFound();
	}

	DestroyAbility();
}

void AKamTargetedAbility::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
}

void AKamTargetedAbility::OverrideValues(EKamTargetType newType, float newMaxRange, float newAngle)
{
	TargetType = newType;
	if (newMaxRange > 0)
		MaxRange = newMaxRange;
	if (newAngle > 0)
		Angle = newAngle;
}

bool AKamTargetedAbility::FindTarget()
{
	switch (TargetType)
	{
	case (EKamTargetType::TT_Caster):
		if (casterUnit != nullptr)
		{
			TargetedUnit = casterUnit;
			return true;
		}
		break;

	case (EKamTargetType::TT_SetTarget):
		if (TargetedUnit != nullptr)
		{
			return true;
		}
		break;

	case (EKamTargetType::TT_ClosestTarget):
		TargetedUnit = GetClosestUnit();
		if (TargetedUnit != nullptr)
		{
			return true;
		}
		break;

	case (EKamTargetType::TT_RandomInRange):
		TargetedUnit = GetRandomUnit();
		if (TargetedUnit != nullptr)
		{
			return true;
		}
		break;

	case (EKamTargetType::TT_ClosestForward):
		TargetedUnit = GetClosestForwardUnit();
		if (TargetedUnit != nullptr)
		{
			return true;
		}
		break;
	}

	return false;
}

TArray<UKamUnit*> AKamTargetedAbility::GetAllUnitsInRange()
{
	TArray<AActor*> Targets;
	TArray<TEnumAsByte<EObjectTypeQuery>> Channel;
	Channel.Add(EObjectTypeQuery::ObjectTypeQuery7);
	Channel.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), MaxRange, Channel, NULL, TArray<AActor*>(), Targets);

	TArray<UKamUnit*> unitsInRange;
	for (AActor* actor : Targets)
	{
		UKamUnit* unitComp = actor->FindComponentByClass<UKamUnit>();
		if (unitComp) 
		{
			if (unitComp->IsUnitHittable())
			{
				if (!unitsToIgnore.Contains(unitComp))
				{
					unitsInRange.Add(unitComp);
				}
			}
		}
	}

	return unitsInRange;
}

UKamUnit* AKamTargetedAbility::GetRandomUnit()
{
	TArray<UKamUnit*> Targets = GetAllUnitsInRange();

	if (Targets.Num() > 0)
	{
		int random = FMath::RandRange(0, Targets.Num() - 1);
		return Targets[random];
	}

	return nullptr;
}

UKamUnit* AKamTargetedAbility::GetClosestUnit()
{
	TArray<UKamUnit*> Targets = GetAllUnitsInRange();
	float nearestDistance = -1;
	UKamUnit* nearestUnit = nullptr;

	for (UKamUnit* unit : Targets)
	{
		// Calculate distance
		float distance = (unit->GetUnitTransform().GetLocation() - GetActorLocation()).Size();

		// Find closest unit
		if (distance < nearestDistance || nearestDistance < 0)
		{
			nearestDistance = distance;
			nearestUnit = unit;
		}
	}

	return nearestUnit;
}

UKamUnit* AKamTargetedAbility::GetClosestForwardUnit()
{
	TArray<UKamUnit*> Targets = GetAllUnitsInRange();
	UKamUnit* nearestUnit = nullptr;
	float nearestDistance = -1;

	FVector forward = GetActorForwardVector();
	float minDot = FMath::Cos(FMath::DegreesToRadians(Angle / 2));

	for (UKamUnit* unit : Targets)
	{
		// Check if Unit is within Angle
		FVector difference = unit->GetUnitTransform().GetLocation() - GetActorLocation();
		FVector diffNormal = difference;
		diffNormal.Normalize();
		float dotProduct = FVector::DotProduct(forward, diffNormal);
		
		if (dotProduct > minDot)
		{
			// Find closest Unit
			float distance = difference.Size();

			if (distance < nearestDistance || nearestDistance < 0)
			{
				nearestDistance = distance;
				nearestUnit = unit;
			}
		}
	}

	UKismetSystemLibrary::DrawDebugConeInDegrees(GetWorld(), GetActorLocation(), forward, MaxRange, Angle / 2, Angle / 2, 12, FLinearColor::Red, 1.0f, 1.0f);

	return nearestUnit;
}

void AKamTargetedAbility::HitHandler(UKamUnit* unitToHit)
{
	if (!unitToHit->IsUnitHittable())
		return;

	// Create Hit Data
	// Create Hit Data
	KamHitData HitData = CreateHitData();

	// Inflict Damage
	unitToHit->TakeDamage(HitData);

	// Call Hit Event
	OnHit(unitToHit);
	if (HitData.AbilityCrit)
		OnCrit(unitToHit);
	if (HitData.TargetDied)
		OnKill(unitToHit);
}

void AKamTargetedAbility::SetTarget(UKamUnit* unit)
{
	if (!unit || !unit->IsUnitHittable())
		return;

	TargetedUnit = unit;
}

UKamUnit* AKamTargetedAbility::GetTarget()
{
	return TargetedUnit;
}
