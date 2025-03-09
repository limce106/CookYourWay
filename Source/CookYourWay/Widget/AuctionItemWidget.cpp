// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/AuctionItemWidget.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/IngredientManagerSystem.h"

void UAuctionItemWidget::NativePreConstruct()
{
	TextBlock_Class = (UTextBlock*)GetWidgetFromName(TEXT("TextBlock_Class"));
	Image_Item_Icon = (UImage*)GetWidgetFromName(TEXT("Image_Item_Icon"));
	TextBlock_Name = (UTextBlock*)GetWidgetFromName(TEXT("TextBlock_Name"));
	TextBlock_HavingNum = (UTextBlock*)GetWidgetFromName(TEXT("TextBlock_HavingNum"));
}

void UAuctionItemWidget::NativeConstruct()
{
	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();
}

int32 UAuctionItemWidget::GetHavingIngrNum(int32 TableIdx)
{
	int32 HavingIngrNum = IngredientManagerSystem->HavingIngrNum[TableIdx];
	return HavingIngrNum;
}

void UAuctionItemWidget::SetClassTextColor()
{
	if (TextBlock_Class->GetText().ToString() == "S") {
		TextBlock_Class->SetColorAndOpacity(FLinearColor(1.f, 0.346704f, 0.097587f, 1.f));
	}
	else if (TextBlock_Class->GetText().ToString() == "A") {
		TextBlock_Class->SetColorAndOpacity(FLinearColor(1.f, 0.590619f, 0.097587f, 1.f));
	}
	else if (TextBlock_Class->GetText().ToString() == "B") {
		TextBlock_Class->SetColorAndOpacity(FLinearColor(0.266356f, 0.266356f, 0.266356f, 1.f));
	}
}

void UAuctionItemWidget::SetItemUI(int32 TableIdx)
{
	FIngrData IngrData = IngredientManagerSystem->IngredientRows[TableIdx];

	TextBlock_Class->SetText(FText::FromString(IngrData.IngrClass));
	Image_Item_Icon->SetBrushFromTexture(IngrData.IngrIcon);
	TextBlock_Name->SetText(FText::FromString(IngrData.IngrName));
	SetClassTextColor();

	SetHavingNumText(TableIdx);
	SetSellingAuctionPrice(TableIdx);
}
