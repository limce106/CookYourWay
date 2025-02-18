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
	

	// 현재 제일 위에 있는 재료의 위치
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
	UFUNCTION(BlueprintCallable)
	TArray<int32> IngrActorToNum();
	UFUNCTION(BlueprintCallable)
	bool IsMeatBurn();
	UFUNCTION(BlueprintCallable)
	void SandwichInteraction();
	UFUNCTION(BlueprintCallable)
	bool IsFirstIngrBread();
	UFUNCTION(BlueprintCallable)
	bool IsLastIngrBread();
	UFUNCTION(BlueprintCallable)
	bool IsCompleteSandwich();

	UFUNCTION(BlueprintImplementableEvent)
	void AddIngredientImg();
	UFUNCTION(BlueprintImplementableEvent)
	void SetIngrWidgetVisibility(ESlateVisibility Visibility);
};
