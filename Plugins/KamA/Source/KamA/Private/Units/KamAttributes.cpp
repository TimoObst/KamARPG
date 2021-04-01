// Fill out your copyright notice in the Description page of Project Settings.

#include "Units/KamAttributes.h"

int FKamAttributes::GetAttribute(EKamAttribute attribute)
{
	if (Attributes.Contains(attribute))
		return Attributes[attribute];

	// if the attribute is not found, always return the base value of 1.
	return 1;
}
