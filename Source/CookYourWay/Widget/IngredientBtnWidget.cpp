// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/IngredientBtnWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include <Kismet/GameplayStatics.h>
#include <Actor/Reuben.h>
#include <GameInstance/IngredientManagerSystem.h>
#include <Blueprint/WidgetBlueprintLibrary.h>
#include "IngredientBoardWidget.h"

void UIngredientBtnWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	Button_Ingredient = (UButton*)GetWidgetFromName(TEXT("Button_Ingredient"));
	TextBlock_IngrName = (UTextBlock*)GetWidgetFromName(TEXT("TextBlock_IngrName"));
}

void UIngredientBtnWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();
	Reuben = Cast<AReuben>(UGameplayStatics::GetPlayerPawn(this, 0));

	Button_Ingredient->OnClicked.AddDynamic(this, &UIngredientBtnWidget::OnClick_ButtonIngredient);
}

void UIngredientBtnWidget::OnClick_ButtonIngredient()
{
	FString IngrName = TextBlock_IngrName->GetText().ToString();
	FString IngrEngName;

	for (int i = 0; i < IngredientManagerSystem->IngredientRows.Num(); i++) {
		if (IngredientManagerSystem->IngredientRows[i]->IngrName == IngrName) {
			IngrEngName = IngredientManagerSystem->IngredientTableRowNames[i].ToString();
			break;
		}
	}

	TArray<UUserWidget*> AllIngredientBoardWidgetArr;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, AllIngredientBoardWidgetArr, BP_IngredientBoardClass);
	UIngredientBoardWidget* IngredientBoardWidget = Cast<UIngredientBoardWidget>(AllIngredientBoardWidgetArr[0]);

	if (!Reuben->IsHold) {
		if (IngredientBoardWidget->IsDessertTab) {
			ADessert* Dessert = GetWorld()->SpawnActor<ADessert>(BP_Dessert, Reuben->GetActorLocation(), Reuben->GetActorRotation());
			Reuben->HoldActor(Dessert);
		}
		else {
			/*³ªÁß¿¡ false·Î ¹Ù²Ü °Í!!*/
			AIngredient* ClickedIngredient = IngredientSpawnFactory::SpawnIngredient(GetWorld(), BP_IngredientClass, Reuben->GetActorLocation(), Reuben->GetActorRotation(), IngrEngName, true);
			Reuben->HoldActor(ClickedIngredient);
		}
	}
	else {
		if (Reuben->HeldActor->GetClass() == BP_Sandwich) {
			// ¼Ò½º¸¦ °ñ¶ú´Ù¸é
			if (IngredientBoardWidget->IsBreadTab || IngredientBoardWidget->IsSauceTab) {
				IngredientBoardWidget->TextBlock_Error->SetVisibility(ESlateVisibility::Hidden);

				/*³ªÁß¿¡ false·Î ¹Ù²Ü °Í!!*/
				AIngredient* ClickedIngredient = IngredientSpawnFactory::SpawnIngredient(GetWorld(), BP_IngredientClass, Reuben->GetActorLocation(), Reuben->GetActorRotation(), IngrEngName, true);
				ASandwich* HoldingSandwich = Cast<ASandwich>(Reuben->HeldActor);
				HoldingSandwich->AddIngredient(ClickedIngredient);
			}
			else {
				IngredientBoardWidget->TextBlock_Error->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else if (Reuben->HeldActor->GetClass() == BP_IngredientClass) {
			IngredientBoardWidget->TextBlock_Error->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
