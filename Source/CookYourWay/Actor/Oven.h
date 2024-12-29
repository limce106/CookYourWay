// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CookingUtensil.h"
#include "Dessert.h"
#include "Oven.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API AOven : public ACookingUtensil
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADessert> BP_Dessert;

	float TempDelayTime;
	bool DelayWithDeltaTime(float DelayTime, float DeltaSeconds);

protected:
	virtual void BeginPlay() override;

	float GetOneCookIncreasement() override;

public:
	virtual void Tick(float DeltaTime) override;
	bool IsBaking = false;

	void PutDessertIn(ADessert* Dessert);
	void Bake();
	bool IsCooked();
	void OvenInteraction();
};
