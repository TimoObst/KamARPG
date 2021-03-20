// Fill out your copyright notice in the Description page of Project Settings.


#include "Units/KamUnitSnapshot.h"

UKamUnitSnapshot::UKamUnitSnapshot()
{
}

UKamUnitSnapshot::~UKamUnitSnapshot()
{
}

void UKamUnitSnapshot::Initialize(TMap<EKamAttribute, int> Attributes, FTransform Transform, float CritChance)
{
	attributes = Attributes;
	transform = Transform;
	critChance = CritChance;
}

int UKamUnitSnapshot::GetAttribute(EKamAttribute attribute)
{
	if (attributes.Contains(attribute))
		return attributes[attribute];

	// if the attribute is not found, always return the base value of 1.
	return 1;
}

FVector UKamUnitSnapshot::GetPosition()
{
	return transform.GetLocation();
}

FQuat UKamUnitSnapshot::GetRotation()
{
	return transform.GetRotation();
}

float UKamUnitSnapshot::GetCritChance()
{
	return critChance;
}
