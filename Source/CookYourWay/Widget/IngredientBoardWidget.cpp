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

	Button_Bread = (UButton*)GetWidgetFromName(TEXT("Button_Bread"));
	Button_Filling = (UButton*)GetWidgetFromName(TEXT("Button_Filling"));
	Button_Meat = (UButton*)GetWidgetFromName(TEXT("Button_Meat"));
	Button_Sauce = (UButton*)GetWidgetFromName(TEXT("Button_Sauce"));
	Button_Dessert = (UButton*)GetWidgetFromName(TEXT("Button_Dessert"));
	UniformGridPanel = (UUniformGridPanel*)GetWidgetFromName(TEXT("UniformGridPanel"));
	TextBlock_Error = (UTextBlock*)GetWidgetFromName(TEXT("TextBlock_Error"));
}

void UIngredientBoardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();

	CreateBreadButtons();

	Button_Bread->OnClicked.AddDynamic(this, &UIngredientBoardWidget::CreateBreadButtons);
	Button_Filling->OnClicked.AddDynamic(this, &UIngredientBoardWidget::CreateFillingButtons);
	Button_Meat->OnClicked.AddDynamic(this, &UIngredientBoardWidget::CreateMeatButtons);
	Button_Sauce->OnClicked.AddDynamic(this, &UIngredientBoardWidget::CreateSauceButtons);
	Button_Dessert->OnClicked.AddDynamic(this, &UIngredientBoardWidget::CreateDessertButtons);
}

void UIngredientBoardWidget::CreateBreadButtons()
{
	IsBreadTab = true;
	IsSauceTab = false;
	IsDessertTab = false;
	UniformGridPanel->ClearChildren();

	int row = 0;
	for (int i = 0; i < IngredientManagerSystem->BreadRows.Num(); i++) {
		if (BP_IngredientBtnWidgetClass) {
			UIngredientBtnWidget* IngredientBtn = CreateWidget<UIngredientBtnWidget>(GetWorld(), BP_IngredientBtnWidgetClass);
			FText IngrName = FText::FromString(IngredientManagerSystem->BreadRows[i]->IngrName);
			IngredientBtn->TextBlock_IngrName->SetText(IngrName);

			if (i % 3 == 0) {
				row++;
			}
			UniformGridPanel->AddChildToUniformGrid(IngredientBtn, row, i % 3);
		}
	}
}

void UIngredientBoardWidget::CreateFillingButtons()
{
	IsBreadTab = false;
	IsSauceTab = false;
	IsDessertTab = false;
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
	IsBreadTab = false;
	IsSauceTab = false;
	IsDessertTab = false;
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
	IsBreadTab = false;
	IsSauceTab = true;
	IsDessertTab = false;
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

void UIngredientBoardWidget::CreateDessertButtons()
{
	IsBreadTab = false;
	IsSauceTab = false;
	IsDessertTab = true;
	UniformGridPanel->ClearChildren();

	if (BP_IngredientBtnWidgetClass) {
		UIngredientBtnWidget* DessertBtn = CreateWidget<UIngredientBtnWidget>(GetWorld(), BP_IngredientBtnWidgetClass);
		FText IngrName = FText::FromString(FString::Printf(TEXT("딸기 케이크")));
		DessertBtn->TextBlock_IngrName->SetText(IngrName);
		UniformGridPanel->AddChildToUniformGrid(DessertBtn, 0, 0);
	}
}
