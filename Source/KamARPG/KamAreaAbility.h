// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"

#include "CoreMinimal.h"
#include "KamBaseAbility.h"
#include "KamAreaAbility.generated.h"

struct KAMARPG_API OverlappedUnit
{
public:
	OverlappedUnit() {};
	OverlappedUnit(UKamUnit* unit)
		: Unit(unit) {};

	UKamUnit* Unit = nullptr;
	float Timer = 0;
};

UCLASS(Abstract)
class KAMARPG_API AKamAreaAbility : public AKamBaseAbility
{
	GENERATED_BODY()
	
protected:
	/// Blueprint Read Only ///
	// Time since Spawn
	UPROPERTY(BlueprintGetter = GetLifeTime)
		float LifeTime = 0;
	
	/// Blueprint Read Write ///
	UPROPERTY(BlueprintReadWrite)
		USphereComponent* SphereCollision = nullptr;
	// Offset the Area Spawnpoint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Area")
		FVector StartPositionOffset = FVector::ZeroVector;
	// Duration of the Area
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Area")
		float Duration = 3.f;
	// Hit Units in Area every TickRate seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Area")
		float TickRate = .5f;

	// Units in the Area
	TArray<OverlappedUnit> overlappedUnits;


	// Initialize the Ability
	void Initialize();

	// Handle hitting Unit
	virtual void HitHandler(UKamUnit* target);

	/// Getter Functions ///
	UFUNCTION(BlueprintGetter)
		float GetLifeTime();

	/// Implementable Blueprint Events ///
	// Event called when Maximum Duration is reached
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability Event")
		void OnMaxDuration();
	// Event called when Unit enters Area
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability Event")
		void OnTargetEnter(UKamUnit* target);
	// Event called when Unit exits Area. Called for Units in Area on Destroy
	UFUNCTION(BlueprintImplementableEvent, Category = "Ability Event")
		void OnTargetExit(UKamUnit* target);

public:
	void BeginPlay() override;
	void Update(float DeltaTime) override;

	void OverrideVariables(float newSize, float newDuration, float newTickRate);

	UFUNCTION()
		void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult); 
	UFUNCTION()
		void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
