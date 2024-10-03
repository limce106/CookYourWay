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

	// 현재 제일 위에 있는 재료의 위치
	FVector CurIngrLoc = GetActorLocation();
	
public:	
	ASandwich();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<AIngredient*> Ingredients;

	void AddIngredient(AIngredient* Ingr);
	//void DetachOneIngr(AIngredient* Ingr);
};
