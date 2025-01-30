// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/FridgeClassGroupWidget.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include <Kismet/GameplayStatics.h>
#include <GameInstance/IngredientManagerSystem.h>

void UFridgeClassGroupWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	TextBlock_Class = (UTextBlock*)GetWidgetFromName(TEXT("TextBlock_Class"));
	UniformGridPanel_Ingr = (UUniformGridPanel*)GetWidgetFromName(TEXT("UniformGridPanel_Ingr"));
}

void UFridgeClassGroupWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();

	TextBlock_Class->SetText(FText::FromString(IngrClass));
}

void UFridgeClassGroupWidget::CreateIngr()
{
	if (IngrType == "Bread") {
		AddIngrPanelChildren(IngredientManagerSystem->BreadRows, IngredientManagerSystem->BreadIdxs, false);
		TextBlock_Class->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (IngrType == "Filling") {
		AddIngrPanelChildren(IngredientManagerSystem->FillingRows, IngredientManagerSystem->FillingIdxs, true);
	}
	else if (IngrType == "Meat") {
		AddIngrPanelChildren(IngredientManagerSystem->MeatRows, IngredientManagerSystem->MeatIdxs, true);
	}
	else if (IngrType == "Sauce") {
		AddIngrPanelChildren(IngredientManagerSystem->SauceRows, IngredientManagerSystem->SauceIdxs, true);
	}
	else if (IngrType == "Dessert") {
		AddIngrPanelChildren(IngredientManagerSystem->DessertRows, IngredientManagerSystem->DessertIdxs, false);
		TextBlock_Class->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UFridgeClassGroupWidget::AddIngrPanelChildren(TArray<FIngrData> IngrDataArr, TArray<int32> IngrTableIdxs, bool IsCheckClass)
{
	const int32 MaxColumn = 4;
	int32 row = 0;
	int32 ConditionSatisfied = 0;
	for (int i = 0; i < IngrDataArr.Num(); i++) {
		if (IsCheckClass && IngrDataArr[i].IngrClass != IngrClass) {
			continue;
		}

		UFridgeIngrWidget* FridgeIngr = CreateWidget<UFridgeIngrWidget>(GetWorld(), BP_FridgeIngr);
		FridgeIngr->AddToViewport();
		FridgeIngr->IngrTableIdx = IngrTableIdxs[i];
		FridgeIngr->CurIngrData = IngredientManagerSystem->IngredientRows[FridgeIngr->IngrTableIdx];
		FridgeIngr->SetIngrUI();

		if (ConditionSatisfied != 0 && ConditionSatisfied % MaxColumn == 0) {
			row++;
		}
		UniformGridPanel_Ingr->AddChildToUniformGrid(FridgeIngr, row, ConditionSatisfied % MaxColumn);
		ConditionSatisfied++;
	}
}
