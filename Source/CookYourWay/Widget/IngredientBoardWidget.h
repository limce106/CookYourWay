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
	void CreateFillingButtons();
	UFUNCTION()
	void CreateMeatButtons();
	UFUNCTION()
	void CreateSauceButtons();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Filling;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Meat;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* Button_Sauce;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UUniformGridPanel* UniformGridPanel;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UIngredientBtnWidget> BP_IngredientBtnWidgetClass;
};
