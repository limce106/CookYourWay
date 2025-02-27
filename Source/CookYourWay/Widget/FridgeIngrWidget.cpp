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

	Image_Ingr = (UImage*)GetWidgetFromName(TEXT("Image_Ingr"));
	Button_Ingredient = (UButton*)GetWidgetFromName(TEXT("Button_Ingredient"));
	TextBlock_IngrName = (UTextBlock*)GetWidgetFromName(TEXT("TextBlock_IngrName"));
	TextBlock_IngrNum = (UTextBlock*)GetWidgetFromName(TEXT("TextBlock_IngrNum"));
}

void UFridgeIngrWidget::NativeConstruct()
{
	Super::NativeConstruct();

	VillageManager = Cast<AVillageManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AVillageManager::StaticClass()));
	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();
	Reuben = Cast<AReuben>(UGameplayStatics::GetPlayerPawn(this, 0));

	Button_Ingredient->OnClicked.RemoveDynamic(this, &UFridgeIngrWidget::OnClick_ButtonIngredient);
	Button_Ingredient->OnClicked.AddDynamic(this, &UFridgeIngrWidget::OnClick_ButtonIngredient);
}

void UFridgeIngrWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (Button_Ingredient) {
		Button_Ingredient->OnClicked.RemoveDynamic(this, &UFridgeIngrWidget::OnClick_ButtonIngredient);
	}
}

void UFridgeIngrWidget::SetIngrUI()
{
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
	// 테스트
	/*if (IngredientManagerSystem->HavingIngrNum[IngrTableIdx] == 0) {
		return;
	}*/

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
		else {
			/*나중에 false로 바꿀 것!!*/
			AIngredient* ClickedIngredient = IngredientSpawnFactory::SpawnIngredient(GetWorld(), BP_IngredientClass, Reuben->GetActorLocation(), Reuben->GetActorRotation(), IngrEngName, true);
			Reuben->HoldActor(ClickedIngredient);
		}
		PayCClassIngr();
	}
	else if (Reuben->IsHold && Reuben->HeldActor->GetClass() == BP_Sandwich) {
		// 소스를 골랐다면
		if (FridgeWidget->CurTabType == ETabType::BreadTab || FridgeWidget->CurTabType == ETabType::SauceTab) {

			/*나중에 false로 바꿀 것!!*/
			AIngredient* ClickedIngredient = IngredientSpawnFactory::SpawnIngredient(GetWorld(), BP_IngredientClass, Reuben->GetActorLocation(), Reuben->GetActorRotation(), IngrEngName, true);
			ASandwich* HoldingSandwich = Cast<ASandwich>(Reuben->HeldActor);
			HoldingSandwich->AddIngredient(ClickedIngredient);
			PayCClassIngr();
		}
		else {
			FridgeWidget->PlayWarningAnim();
		}
		
	}
	else if (Reuben->IsHold && Reuben->HeldActor->GetClass() == BP_IngredientClass) {
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
}
