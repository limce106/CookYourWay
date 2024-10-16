// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/IngredientBoardWidget.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Components/TextBlock.h"
#include <Kismet/GameplayStatics.h>
#include <GameInstance/IngredientManagerSystem.h>

void UIngredientBoardWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	Button_Filling = (UButton*)GetWidgetFromName(TEXT("Button_Filling"));
	Button_Meat = (UButton*)GetWidgetFromName(TEXT("Button_Meat"));
	Button_Sauce = (UButton*)GetWidgetFromName(TEXT("Button_Sauce"));
	UniformGridPanel = (UUniformGridPanel*)GetWidgetFromName(TEXT("UniformGridPanel"));
}

void UIngredientBoardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();

	CreateFillingButtons();

	Button_Filling->OnClicked.AddDynamic(this, &UIngredientBoardWidget::CreateFillingButtons);
	Button_Meat->OnClicked.AddDynamic(this, &UIngredientBoardWidget::CreateMeatButtons);
	Button_Sauce->OnClicked.AddDynamic(this, &UIngredientBoardWidget::CreateSauceButtons);
}

void UIngredientBoardWidget::CreateFillingButtons()
{
	UniformGridPanel->ClearChildren();

	int row = 0;
	for (int i = 0; i < IngredientManagerSystem->FillingRows.Num(); i++) {
		if (BP_IngredientBtnWidgetClass) {
			UIngredientBtnWidget* IngredientBtn = CreateWidget<UIngredientBtnWidget>(GetWorld(), BP_IngredientBtnWidgetClass);
			FText IngrName = FText::FromString(IngredientManagerSystem->FillingRows[i]->IngrName);
			IngredientBtn->TextBlock_IngrName->SetText(IngrName);

			if (i % 3 == 0) {
				row++;
			}
			UniformGridPanel->AddChildToUniformGrid(IngredientBtn, row, i % 3);
		}
	}
}

void UIngredientBoardWidget::CreateMeatButtons()
{
	UniformGridPanel->ClearChildren();

	int row = 0;
	for (int i = 0; i < IngredientManagerSystem->MeatRows.Num(); i++) {
		if (BP_IngredientBtnWidgetClass) {
			UIngredientBtnWidget* IngredientBtn = CreateWidget<UIngredientBtnWidget>(GetWorld(), BP_IngredientBtnWidgetClass);
			FText IngrName = FText::FromString(IngredientManagerSystem->MeatRows[i]->IngrName);
			IngredientBtn->TextBlock_IngrName->SetText(IngrName);

			if (i % 3 == 0) {
				row++;
			}
			UniformGridPanel->AddChildToUniformGrid(IngredientBtn, row, i % 3);
		}
	}
}

void UIngredientBoardWidget::CreateSauceButtons()
{
	UniformGridPanel->ClearChildren();

	int row = 0;
	for (int i = 0; i < IngredientManagerSystem->SauceRows.Num(); i++) {
		if (BP_IngredientBtnWidgetClass) {
			UIngredientBtnWidget* IngredientBtn = CreateWidget<UIngredientBtnWidget>(GetWorld(), BP_IngredientBtnWidgetClass);
			FText IngrName = FText::FromString(IngredientManagerSystem->SauceRows[i]->IngrName);
			IngredientBtn->TextBlock_IngrName->SetText(IngrName);

			if (i % 3 == 0) {
				row++;
			}
			UniformGridPanel->AddChildToUniformGrid(IngredientBtn, row, i % 3);
		}
	}
}
