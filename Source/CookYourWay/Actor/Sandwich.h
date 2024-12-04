// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ingredient.h"
#include "Sandwich.generated.h"

UCLASS()
class COOKYOURWAY_API ASandwich : public AActor
{
	GENERATED_BODY()

	class UIngredientManagerSystem* IngredientManagerSystem;

	// ���� ���� ���� �ִ� ����� ��ġ
	float LastIngrLocZ;
	
public:	
	ASandwich();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> Ingredients;

	void AddIngredient(AIngredient* Ingr);

	void DestroySandwich();
	//void DetachOneIngr(AIngredient* Ingr);
};
