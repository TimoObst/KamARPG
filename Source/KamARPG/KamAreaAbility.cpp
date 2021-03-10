// Fill out your copyright notice in the Description page of Project Settings.

#include "KamAreaAbility.h"

void AKamAreaAbility::Initialize()
{
	// Set object size
	SetSize(Size);

	// Spawn Overlap
	TArray<AActor*> overlappingActors;
	GetOverlappingActors(overlappingActors);
	for (AActor* OtherActor : overlappingActors)
	{
		UKamUnit* kamUnit = OtherActor->FindComponentByClass<UKamUnit>();
		if (kamUnit != nullptr)
		{
			overlappedUnits.Add(OverlappedUnit(kamUnit));
		}
	}

	// setup collision detection
	if (SphereCollision != nullptr)
	{
		SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AKamAreaAbility::OnSphereBeginOverlap);
		SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AKamAreaAbility::OnSphereEndOverlap);
	}
}

void AKamAreaAbility::HitHandler(UKamUnit* target)
{
	// Check if target already hit
	if (!target->IsUnitHittable())
		return;

	// Create Hit Data
	KamHitData HitData = CreateHitData();

	// Inflict Damage
	target->TakeDamage(HitData);

	// Call Hit Event
	OnHit(target);
	if (HitData.AbilityCrit)
		OnCrit(target);
	if (HitData.TargetDied)
		OnKill(target);
}

void AKamAreaAbility::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void AKamAreaAbility::Update(float DeltaTime)
{
	// Tick on Overlapping Enemies
	for (OverlappedUnit& unit : overlappedUnits)
	{
		unit.Timer -= DeltaTime;

		if (unit.Timer <= 0)
		{
			HitHandler(unit.Unit);
			unit.Timer += TickRate;
		}
	}

	// Increase Duration
	LifeTime += DeltaTime;

	if (LifeTime >= Duration)
	{
		OnMaxDuration();
		DestroyAbility();
	}

	Super::Update(DeltaTime);
}

void AKamAreaAbility::OverrideVariables(float newSize, float newDuration, float newTickRate)
{
	if (newSize > 0)
		SetSize(newSize);
	if (newDuration > 0)
		Duration = newDuration;
	if (newTickRate > 0)
		TickRate = newTickRate;
}

void AKamAreaAbility::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player")) return;

	UKamUnit* kamUnit = OtherActor->FindComponentByClass<UKamUnit>();
	if (kamUnit != nullptr)
	{
		overlappedUnits.Add(OverlappedUnit(kamUnit));
	}
}

void AKamAreaAbility::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Player")) return;

	UKamUnit* kamUnit = OtherActor->FindComponentByClass<UKamUnit>();
	if (kamUnit != nullptr)
	{
		for (int i = 0; i < overlappedUnits.Num(); ++i)
		{
			if (overlappedUnits[i].Unit == kamUnit)
			{
				overlappedUnits.RemoveAt(i);
				break;
			}
		}
	}
}

float AKamAreaAbility::GetLifeTime()
{
	return LifeTime;
}
