// Fill out your copyright notice in the Description page of Project Settings.

#include "KamExplosionAbility.h"

#include <Runtime/Engine/Classes/Engine/Engine.h>

void AKamExplosionAbility::Initialize()
{
	// Move to Start Position
	FVector startPosition = GetActorLocation() + (GetActorRotation().RotateVector(StartPositionOffset));
	SetActorLocation(startPosition);

	// set starting size
	SetSize(StartSize);

	// setup collision detection
	if (SphereCollision != nullptr)
	{
		SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AKamExplosionAbility::OnSphereBeginOverlap);
	}
}

void AKamExplosionAbility::HitHandler(UKamUnit* target)
{
	// Check if target already hit
	if (unitsToIgnore.Contains(target) || !target->IsUnitHittable())
		return;

	// Inflict Damage
	KamHitData HitData = CreateHitData();
	target->TakeDamage(HitData);

	// Call Events
	OnHit(target);
	if (HitData.AbilityCrit)
		OnCrit(target);
	if (HitData.TargetDied)
		OnKill(target);

	// Add to targets hit
	unitsToIgnore.Add(target);
}

void AKamExplosionAbility::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
}

void AKamExplosionAbility::Update(float DeltaTime)
{
	if (lifeTime > Duration)
	{
		OnMaxSize();
		DestroyAbility();
		return;
	}

	lifeTime += DeltaTime;

	// Grow Explosion
	float sizeDifference = EndSize - StartSize;
	float sizeChange = sizeDifference / Duration * DeltaTime;
	SetSize(Size + sizeChange);

	Super::Update(DeltaTime);
}

void AKamExplosionAbility::OverrideVariables(float newStartSize, float newEndSize, float newLifeSpan)
{
	if (newStartSize > 0)
		StartSize = newStartSize;
	if (newEndSize > 0)
		EndSize = newEndSize;
	if (newLifeSpan > 0)
		Duration = newLifeSpan;
}

void AKamExplosionAbility::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player")) return;

	UKamUnit* kamUnit = OtherActor->FindComponentByClass<UKamUnit>();
	if (kamUnit != nullptr)
		HitHandler(kamUnit);
}
