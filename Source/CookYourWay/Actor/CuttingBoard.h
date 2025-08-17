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

	const int32 CutCount = 15;
	float IngredientShrinkRate;
	UStaticMeshComponent* IngrStaticMesh;

	void SetIngredientShrinkRate();
	
protected:
	virtual void BeginPlay() override;

	float GetCookIncreasement() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void PutIngrOn(AIngredient* Ingr) override;
	UFUNCTION()
	void Chop();
	UFUNCTION()
	void CuttingBoardInteraction();
};
