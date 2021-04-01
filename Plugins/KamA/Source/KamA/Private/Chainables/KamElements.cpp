// Fill out your copyright notice in the Description page of Project Settings.


#include "Chainables/KamElements.h"

KamElements::KamElements()
{
}

KamElements::~KamElements()
{
}

FString KamElements::GetElementEnumAsString(EElements element)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EElements"), true);
	if (!EnumPtr) return FString("Invalid");

	return EnumPtr->GetNameByValue((int64)element).ToString();
}
