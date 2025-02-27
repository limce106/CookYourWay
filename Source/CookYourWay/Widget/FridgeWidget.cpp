// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/FridgeWidget.h"
#include <Kismet/GameplayStatics.h>
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "GameInstance/IngredientManagerSystem.h"
#include "FridgeClassGroupWidget.h"

void UFridgeWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	Button_Bread = (UButton*)GetWidgetFromName(TEXT("Button_Bread"));
	Button_Filling = (UButton*)GetWidgetFromName(TEXT("Button_Filling"));
	Button_Meat = (UButton*)GetWidgetFromName(TEXT("Button_Meat"));
	Button_Sauce = (UButton*)GetWidgetFromName(TEXT("Button_Sauce"));
	Button_Dessert = (UButton*)GetWidgetFromName(TEXT("Button_Dessert"));
	ScrollBox_Ingredients = (UScrollBox*)GetWidgetFromName(TEXT("ScrollBox_Ingredients"));
}

void UFridgeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();

	Button_Bread->OnClicked.AddDynamic(this, &UFridgeWidget::CreateBreadButtons);
	Button_Filling->OnClicked.AddDynamic(this, &UFridgeWidget::CreateFillingButtons);
	Button_Meat->OnClicked.AddDynamic(this, &UFridgeWidget::CreateMeatButtons);
	Button_Sauce->OnClicked.AddDynamic(this, &UFridgeWidget::CreateSauceButtons);
	Button_Dessert->OnClicked.AddDynamic(this, &UFridgeWidget::CreateDessertButtons);

	CreateBreadButtons();
}

void UFridgeWidget::CreateBreadButtons()
{
	CurTabType = ETabType::BreadTab;
	ScrollBox_Ingredients->ClearChildren();

	UFridgeClassGroupWidget* FridgeClassGroup = CreateWidget<UFridgeClassGroupWidget>(GetWorld(), BP_FridgeClassGroup);
	FridgeClassGroup->AddToViewport();
	FridgeClassGroup->IngrClass = IngredientManagerSystem->BreadRows[0].IngrClass;
	FridgeClassGroup->IngrType = IngredientManagerSystem->BreadRows[0].IngrType;
	FridgeClassGroup->CreateIngr();

	ScrollBox_Ingredients->AddChild(FridgeClassGroup);
}

void UFridgeWidget::CreateFillingButtons()
{
	CurTabType = ETabType::FillingTab;
	ScrollBox_Ingredients->ClearChildren();

	for (int i = 0; i < ClassName.Num(); i++) {
		UFridgeClassGroupWidget* FridgeClassGroup = CreateWidget<UFridgeClassGroupWidget>(GetWorld(), BP_FridgeClassGroup);
		FridgeClassGroup->AddToViewport();
		FridgeClassGroup->IngrClass = ClassName[i];
		FridgeClassGroup->IngrType = IngredientManagerSystem->FillingRows[i].IngrType;
		FridgeClassGroup->CreateIngr();

		ScrollBox_Ingredients->AddChild(FridgeClassGroup);
	}
}

void UFridgeWidget::CreateMeatButtons()
{
	CurTabType = ETabType::MeatTab;
	ScrollBox_Ingredients->ClearChildren();

	for (int i = 0; i < ClassName.Num(); i++) {
		UFridgeClassGroupWidget* FridgeClassGroup = CreateWidget<UFridgeClassGroupWidget>(GetWorld(), BP_FridgeClassGroup);
		FridgeClassGroup->AddToViewport();
		FridgeClassGroup->IngrClass = ClassName[i];
		FridgeClassGroup->IngrType = IngredientManagerSystem->MeatRows[i].IngrType;
		FridgeClassGroup->CreateIngr();

		ScrollBox_Ingredients->AddChild(FridgeClassGroup);
	}
}

void UFridgeWidget::CreateSauceButtons()
{
	CurTabType = ETabType::SauceTab;
	ScrollBox_Ingredients->ClearChildren();

	for (int i = 0; i < ClassName.Num(); i++) {
		UFridgeClassGroupWidget* FridgeClassGroup = CreateWidget<UFridgeClassGroupWidget>(GetWorld(), BP_FridgeClassGroup);
		FridgeClassGroup->AddToViewport();
		FridgeClassGroup->IngrClass = ClassName[i];
		FridgeClassGroup->IngrType = IngredientManagerSystem->SauceRows[i].IngrType;
		FridgeClassGroup->CreateIngr();

		ScrollBox_Ingredients->AddChild(FridgeClassGroup);
	}
}

void UFridgeWidget::CreateDessertButtons()
{
	CurTabType = ETabType::DessertTab;
	ScrollBox_Ingredients->ClearChildren();

	UFridgeClassGroupWidget* FridgeClassGroup = CreateWidget<UFridgeClassGroupWidget>(GetWorld(), BP_FridgeClassGroup);
	FridgeClassGroup->AddToViewport();
	FridgeClassGroup->IngrClass = IngredientManagerSystem->DessertRows[0].IngrClass;
	FridgeClassGroup->IngrType = IngredientManagerSystem->DessertRows[0].IngrType;
	FridgeClassGroup->CreateIngr();

	ScrollBox_Ingredients->AddChild(FridgeClassGroup);
}
