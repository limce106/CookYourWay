// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Widget/CookRateWidget.h"
#include "Sandwich.h"
#include "CookingUtensil.generated.h"

UCLASS()
class COOKYOURWAY_API ACookingUtensil : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASandwich> BP_Sandwich;

	class AReuben* Reuben;
	
public:	
	ACookingUtensil();

protected:
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	UCookRateWidget* BP_CookRateWidget;

	// 조리도구 위에 재료가 올라갔는지
	UPROPERTY(BlueprintReadWrite)
	bool IsIngredientOn = false;
	// 조리도구 위에 올라간 재료 액터
	UPROPERTY(BlueprintReadWrite)
	class AIngredient* PlacedIngredient;

	virtual float GetOneCookIncreasement();

	virtual void PutIngrOn(AIngredient* Ingr);
	void PickUpIngr();
};
