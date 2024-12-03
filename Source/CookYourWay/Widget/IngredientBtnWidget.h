// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Actor/Ingredient.h>
#include "IngredientBtnWidget.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API UIngredientBtnWidget : public UUserWidget
{
	GENERATED_BODY()

	class UIngredientManagerSystem* IngredientManagerSystem;
	class AReuben* Reuben;

	void NativePreConstruct();
	void NativeConstruct();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Ingredient;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_IngrName;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AIngredient> BP_IngredientClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> BP_IngredientBoardClass;

	UFUNCTION()
	void OnClick_ButtonIngredient();
};
