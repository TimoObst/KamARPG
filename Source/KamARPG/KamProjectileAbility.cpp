// Fill out your copyright notice in the Description page of Project Settings.


#include "KamProjectileAbility.h"
#include "KamHitData.h"

#include "Runtime/Engine/Classes/Engine/Engine.h"

void AKamProjectileAbility::Initialize()
{
	// Move to Start Position
	startPosition = GetActorLocation() + (GetActorRotation().RotateVector(StartPositionOffset));
	SetActorLocation(startPosition);
	// Rotate based on Angle
	SetActorRotation(GetActorForwardVector().RotateAngleAxis(Angle, GetActorUpVector()).Rotation());

	// Set object size
	SetSize(Size);

	// setup projectile movement
	if (ProjectileMovement == nullptr)
	{
		ProjectileMovement = NewObject<UProjectileMovementComponent>(this, "Projectile Movement Comp");
		ProjectileMovement->RegisterComponent();
	}

	SetSpeed(Speed);
	ProjectileMovement->ProjectileGravityScale = 0;

	// setup collision detection
	if (SphereCollision != nullptr)
	{
		SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AKamProjectileAbility::OnSphereBeginOverlap);
	}
}

void AKamProjectileAbility::HitHandler(UKamUnit* target)
{
	// Check if target already hit
	if (unitsToIgnore.Contains(target) || !target->IsUnitHittable())
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

	// Destroy if needed
	if (DestroyOnHit)
	{
		DestroyAbility();
		return;
	}

	// Add to targets hit
	unitsToIgnore.Add(target);
}

void AKamProjectileAbility::BeginPlay()
{
	Initialize();

	Super::BeginPlay();
}

void AKamProjectileAbility::Update(float DeltaTime)
{
	// check distance traveled
	Distance = abs((GetActorLocation() - startPosition).Size());
	
	if (Distance > MaxDistance)
	{
		OnMaxDistance();
		DestroyAbility();
	}

	Super::Update(DeltaTime);
}

void AKamProjectileAbility::OverrrideVariables(float newSize, float newAngle, float newSpeed, float newMaxDistance)
{
	if (newSize > 0)
		SetSize(newSize);
	if (newAngle > 0)
		Angle = newAngle;
	if (newSpeed > 0)
		Speed = newSpeed;
	if (newMaxDistance > 0)
		MaxDistance = newMaxDistance;
}

void AKamProjectileAbility::SetSpeed(float newSpeed)
{
	Speed = newSpeed;

	if (!ProjectileMovement) return;

	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->Velocity = GetActorForwardVector() * Speed;
}

void AKamProjectileAbility::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Player") || OtherActor->ActorHasTag("KamAbility")) return;

	UKamUnit* kamUnit = OtherActor->FindComponentByClass<UKamUnit>();
	if (kamUnit != nullptr)
		HitHandler(kamUnit);
	else
		DestroyAbility();
}
