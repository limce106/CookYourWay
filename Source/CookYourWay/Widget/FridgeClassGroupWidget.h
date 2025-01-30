// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FridgeIngrWidget.h"
#include "FridgeClassGroupWidget.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API UFridgeClassGroupWidget : public UUserWidget
{
	GENERATED_BODY()

	class UIngredientManagerSystem* IngredientManagerSystem;

	void NativePreConstruct();
	void NativeConstruct();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_Class;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UUniformGridPanel* UniformGridPanel_Ingr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFridgeIngrWidget> BP_FridgeIngr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FString IngrClass;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FString IngrType;
	
	UFUNCTION(BlueprintCallable)
	void CreateIngr();
	UFUNCTION()
	void AddIngrPanelChildren(TArray<FIngrData> IngrDataArr, TArray<int32> IngrTableIdxs, bool IsCheckClass);
};
