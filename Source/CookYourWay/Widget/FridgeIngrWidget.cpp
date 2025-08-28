// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/FridgeIngrWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include <Kismet/GameplayStatics.h>
#include <Actor/VillageManager.h>
#include <Actor/Reuben.h>
#include <Actor/PlayerBistro.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

void UFridgeIngrWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

}

void UFridgeIngrWidget::NativeConstruct()
{
	Super::NativeConstruct();

	VillageManager = Cast<AVillageManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AVillageManager::StaticClass()));
	Reuben = Cast<AReuben>(UGameplayStatics::GetPlayerPawn(this, 0));
	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();

	Image_Ingr = (UImage*)GetWidgetFromName(TEXT("Image_Ingr"));
	Button_Ingredient = (UButton*)GetWidgetFromName(TEXT("Button_Ingredient"));
	TextBlock_IngrName = (UTextBlock*)GetWidgetFromName(TEXT("TextBlock_IngrName"));
	TextBlock_IngrNum = (UTextBlock*)GetWidgetFromName(TEXT("TextBlock_IngrNum"));

	Button_Ingredient->OnClicked.AddDynamic(this, &UFridgeIngrWidget::OnClick_ButtonIngredient);
}

void UFridgeIngrWidget::SetIngrUI()
{
	FString CurrentLevel = GetWorld()->GetMapName();
	if (CurrentLevel.Contains("LV_Auction"))
	{
		Button_Ingredient->SetIsEnabled(false);
	}

	Image_Ingr->SetBrushFromTexture(CurIngrData.IngrIcon);
	TextBlock_IngrName->SetText(FText::FromString(CurIngrData.IngrName));

	if (CurIngrData.IngrClass == "C") {
		FString IngrPrice = TEXT("$ ");
		IngrPrice.Append(FString::FromInt(IngredientManagerSystem->GetIngrBidMin("C")));
		TextBlock_IngrNum->SetText(FText::FromString(IngrPrice));
	}
	else {
		FString HavingNum = FString::FromInt(IngredientManagerSystem->HavingIngrNum[IngrTableIdx]);
		TextBlock_IngrNum->SetText(FText::FromString(HavingNum));
	}
}

void UFridgeIngrWidget::OnClick_ButtonIngredient()
{
	if (IngredientManagerSystem->HavingIngrNum[IngrTableIdx] == 0) {
		return;
	}

	FString IngrEngName = IngredientManagerSystem->IngredientTableRowNames[IngrTableIdx].ToString();

	TArray<UUserWidget*> AllFridgeWidgetArr;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, AllFridgeWidgetArr, BP_FrideWidget);
	UFridgeWidget* FridgeWidget = Cast<UFridgeWidget>(AllFridgeWidgetArr[0]);

	if (!FridgeWidget->CanBuyIngr) {
		return;
	}

	if (!Reuben->IsHold) {
		if (FridgeWidget->CurTabType == ETabType::DessertTab) {
			ADessert* Dessert = GetWorld()->SpawnActor<ADessert>(BP_Dessert, Reuben->GetActorLocation(), Reuben->GetActorRotation());
			Reuben->HoldActor(Dessert);
		}
		else if (FridgeWidget->CurTabType == ETabType::BreadTab) {
			FridgeWidget->PlayWarningAnim();
		}
		else {
			AIngredient* ClickedIngredient = IngredientSpawnFactory::SpawnIngredient(GetWorld(), BP_IngredientClass, Reuben->GetActorLocation(), Reuben->GetActorRotation(), IngrEngName, false);
			Reuben->HoldActor(ClickedIngredient);
		}
		PayCClassIngr();
	}
	else if (Reuben->IsHold && Reuben->HeldActor->GetClass() == BP_Sandwich) {
		// 빵이나 소스를 골랐다면
		if (FridgeWidget->CurTabType == ETabType::BreadTab || FridgeWidget->CurTabType == ETabType::SauceTab) {

			AIngredient* ClickedIngredient = IngredientSpawnFactory::SpawnIngredient(GetWorld(), BP_IngredientClass, Reuben->GetActorLocation(), Reuben->GetActorRotation(), IngrEngName, false);
			ASandwich* HoldingSandwich = Cast<ASandwich>(Reuben->HeldActor);
			HoldingSandwich->AddIngredient(ClickedIngredient);
			PayCClassIngr();
		}
		else {
			FridgeWidget->PlayWarningAnim();
		}
		
	}
	else {
		FridgeWidget->PlayWarningAnim();
	}
}

void UFridgeIngrWidget::PayCClassIngr()
{
	int32 IngrPrice = IngredientManagerSystem->GetIngrBinMinByIndex(IngrTableIdx);
	Reuben->PlayerBistro->TodayNetIncome -= IngrPrice;

	if (CurIngrData.IngrClass == "C") {
		VillageManager->VillageManagerSystem->TotalAsset -= IngrPrice;
	}

	MinusHavingIngrNum();
}

void UFridgeIngrWidget::MinusHavingIngrNum()
{
	if (CurIngrData.IngrClass == "C")
		return;

	IngredientManagerSystem->HavingIngrNum[IngrTableIdx]--;
	FString HavingNum = FString::FromInt(IngredientManagerSystem->HavingIngrNum[IngrTableIdx]);
	TextBlock_IngrNum->SetText(FText::FromString(HavingNum));
}
