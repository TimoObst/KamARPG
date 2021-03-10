// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

#include "CoreMinimal.h"
#include "KamBaseAbility.h"
#include "KamProjectileAbility.generated.h"


UCLASS(Abstract)
class KAMARPG_API AKamProjectileAbility : public AKamBaseAbility
{
	GENERATED_BODY()
	
protected:
	FVector startPosition = FVector::ZeroVector;

	/// Blueprint Read Only ///
	// Distance from Spawn Point
	UPROPERTY(BlueprintReadOnly)
		float Distance = 0;
	UPROPERTY(BlueprintReadOnly)
		UProjectileMovementComponent* ProjectileMovement = nullptr;

	/// Blueprint Read Write ///
	UPROPERTY(BlueprintReadWrite, Category = "Ability\|Component")
		USphereComponent* SphereCollision = nullptr;
	// Direction the Projectile will go in after Spawn. 0 = Forward
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Projectile")
		float Angle = 0;
	// Movement Speed of the Projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Projectile", BlueprintSetter = SetSpeed)
		float Speed = 600;
	// Maximum Distance the Projectile can travel from Spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Projectile")
		float MaxDistance = 1000;
	// Projectile will spawn with an Offset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Projectile")
		FVector StartPositionOffset = FVector(20, 0, 0);
	// Projectile can only hit 1 Unit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability\|Projectile")
		bool DestroyOnHit = true;

	virtual void HitHandler(UKamUnit* target) override;

	void Initialize();

	/// Blueprint Implementable Events ///
	// Event called when the Projectile reaches Maximum Distance
	UFUNCTION(BlueprintImplementableEvent, Category = "Kam Event")
		void OnMaxDistance();

	/// Blueprint Callable Functions ///
	// Change the Speed of the Projectile
	UFUNCTION(BlueprintCallable, Category = "Ability Component")
		void SetSpeed(float newSpeed);

public:
	virtual void BeginPlay() override;
	virtual void Update(float DeltaTime) override;

	void OverrrideVariables(float newSize, float newAngle, float newSpeed, float newMaxDistance);

	UFUNCTION()
		void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
