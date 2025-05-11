// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/CookingUtensil.h"
#include "FryPan.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API AFryPan : public ACookingUtensil
{
	GENERATED_BODY()
	
	class UParticleSystemComponent* ParticleSystemComponent;

	float OverCookedTime = 3.0f;

	float TempDelayTime;
	bool DelayWithDeltaTime(float DelayTime, float DeltaSeconds);
	
protected:
	virtual void BeginPlay() override;

	float GetCookIncreasement() override;

public:
	virtual void Tick(float DeltaTime) override;


	bool IsFrying = false;

	virtual void PutIngrOn(AIngredient* Ingr) override;
	UFUNCTION()
	void Fry();
	UFUNCTION()
	void FryPanInteraction();

	UFUNCTION()
	void PlacedIngredientBurnt();
};
