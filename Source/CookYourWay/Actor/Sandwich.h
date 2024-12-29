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
	USceneComponent* DefaultRootComponent;

	// ���� ���� ���� �ִ� ����� ��ġ
	float LastIngrLocZ;
	
public:	
	ASandwich();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<AIngredient*> Ingredients;

	UFUNCTION(BlueprintCallable)
	void AddIngredient(AIngredient* Ingr);

	UFUNCTION(BlueprintCallable)
	void DestroySandwich();

	TArray<int32> IngrActorToNum();

	bool IsMeatBurn();
	void SandwichInteraction();

	bool IsFirstIngrBread();
	bool IsLastIngrBread();
};
