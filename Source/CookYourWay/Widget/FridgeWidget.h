// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FridgeWidget.generated.h"

UENUM(BlueprintType)
enum class ETabType : uint8 {
	BreadTab	UMETA(DisplayName = "BreadTab"),
	FillingTab	UMETA(DisplayName = "FillingTab"),
	MeatTab	UMETA(DisplayName = "MeatTab"),
	SauceTab	UMETA(DisplayName = "SauceTab"),
	DessertTab	UMETA(DisplayName = "DessertTab"),
};

UCLASS()
class COOKYOURWAY_API UFridgeWidget : public UUserWidget
{
	GENERATED_BODY()

	class UIngredientManagerSystem* IngredientManagerSystem;
	const TArray<FString> ClassName = { "S", "A", "B", "C" };

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
	class UScrollBox* ScrollBox_Ingredients;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_Error;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> BP_FridgeClassGroup;

	UPROPERTY(BlueprintReadWrite)
	bool CanBuyIngr = true;

	ETabType CurTabType = ETabType::BreadTab;
};
