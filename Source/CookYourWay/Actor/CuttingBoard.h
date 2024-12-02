// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/CookingUtensil.h"
#include "CuttingBoard.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API ACuttingBoard : public ACookingUtensil
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	float GetOneCookIncreasement() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void PutIngrOn(AIngredient* Ingr) override;
	void Chop();
};
