// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Chainables/Abilities/KamBaseAbility.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KamCharacter.generated.h"

UCLASS(Blueprintable)
class AKamCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AKamCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	/* Abilities */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		USceneComponent* AbilitySpawnPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
		TMap<FString, TSubclassOf<AKamBaseAbility>> Abilities;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
		UKamUnit* unitRef = nullptr;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;

	void OnCastAbility(FString Key);
	DECLARE_DELEGATE_OneParam(FOnCastAbilityDelegate, FString);

};

