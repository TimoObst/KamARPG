// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"

#include "CoreMinimal.h"
#include "Chainables/Abilities/KamBaseAbility.h"
#include "KamExplosionAbility.generated.h"


UCLASS(Abstract)
class KAMA_API AKamExplosionAbility : public AKamBaseAbility
{
	GENERATED_BODY()
	
protected:
	/// Blueprint Read Only ///
	// Time since Spawn
	UPROPERTY(BlueprintReadOnly)
		float lifeTime = 0;

	/// Blueprint Read Write ///
	UPROPERTY(BlueprintReadWrite, Category = "Ability\|Component")
		USphereComponent* SphereCollision = nullptr;
	// Size of the Explosion on Start
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Explosion")
		float StartSize = 1;
	// Size of the Explosion on End
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Explosion")
		float EndSize = 2;
	// Offset the Explosion Spawnpoint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Explosion")
		FVector StartPositionOffset = FVector::ZeroVector;
	// Duration of the Explosion
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Explosion")
		float Duration = .5f;

	void Initialize();
	virtual void Update(float DeltaTime) override;

	virtual void HitHandler(UKamUnit* target) override;

public:
	virtual void BeginPlay() override;

	void OverrideVariables(float newStartSize, float newEndSize, float newLifeSpan);

	/// Blueprint Implementable Events ///
	// Event called when Maximum Size is reached
	UFUNCTION(BlueprintImplementableEvent, Category = "Kam Event")
		void OnMaxSize();

	/// Collision Detection ///
	UFUNCTION()
		void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
