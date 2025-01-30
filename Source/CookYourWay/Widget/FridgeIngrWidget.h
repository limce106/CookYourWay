// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <GameInstance/IngredientManagerSystem.h>
#include "FridgeWidget.h"
#include <Actor/Ingredient.h>
#include <Actor/Sandwich.h>
#include <Actor/Dessert.h>
#include "FridgeIngrWidget.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API UFridgeIngrWidget : public UUserWidget
{
	GENERATED_BODY()

	class AVillageManager* VillageManager;
	class UIngredientManagerSystem* IngredientManagerSystem;
	class AReuben* Reuben;
	
	void NativePreConstruct();
	void NativeConstruct();
	void NativeDestruct();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_Ingr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Ingredient;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_IngrName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_IngrNum;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFridgeWidget> BP_FrideWidget;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AIngredient> BP_IngredientClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASandwich> BP_Sandwich;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADessert> BP_Dessert;

	FIngrData CurIngrData;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	int32 IngrTableIdx;

	UFUNCTION(BlueprintCallable)
	void SetIngrUI();
	UFUNCTION()
	void OnClick_ButtonIngredient();
	UFUNCTION()
	void PayCClassIngr();
};
