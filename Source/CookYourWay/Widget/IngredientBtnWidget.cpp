// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/IngredientBtnWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include <Kismet/GameplayStatics.h>
#include <Actor/Reuben.h>
#include <GameInstance/IngredientManagerSystem.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

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

	if (!Reuben->IsHold) {
		AIngredient* Ingredient = GetWorld()->SpawnActor<AIngredient>(BP_IngredientClass, Reuben->GetActorLocation(), Reuben->GetActorRotation());
		Ingredient->StaticMesh->SetStaticMesh(IngredientManagerSystem->GetIngrModel(IngrEngName));
		Reuben->AttachToSocket(Ingredient);

		TArray<UUserWidget*> AllIngredientBoardWidgetArr;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, AllIngredientBoardWidgetArr, BP_IngredientBoardClass);
		AllIngredientBoardWidgetArr[0]->RemoveFromParent();
	}
}
