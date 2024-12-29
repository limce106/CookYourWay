// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IngredientBtnWidget.h"
#include "IngredientBoardWidget.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API UIngredientBoardWidget : public UUserWidget
{
	GENERATED_BODY()

	class UIngredientManagerSystem* IngredientManagerSystem;
	
	void NativePreConstruct();
	void NativeConstruct();

protected:
	UFUNCTION()
	void CreateBreadButtons();
	UFUNCTION()
	void CreateFillingButtons();
	UFUNCTION()
	void CreateMeatButtons();
	UFUNCTION()
	void CreateSauceButtons();
	UFUNCTION()
	void CreateDessertButtons();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Bread;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Filling;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Meat;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Sauce;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Dessert;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UUniformGridPanel* UniformGridPanel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_Error;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UIngredientBtnWidget> BP_IngredientBtnWidgetClass;

	bool IsBreadTab= false;
	bool IsSauceTab = false;
	bool IsDessertTab = false;
};
