// Fill out your copyright notice in the Description page of Project Settings.


#include "KamARPG_Elements.h"

KamARPG_Elements::KamARPG_Elements()
{
}

KamARPG_Elements::~KamARPG_Elements()
{
}

FString KamARPG_Elements::GetElementEnumAsString(EElements element)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EElements"), true);
	if (!EnumPtr) return FString("Invalid");

	return EnumPtr->GetNameByValue((int64)element).ToString();
}
