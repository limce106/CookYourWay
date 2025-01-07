// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "AuctionItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API UAuctionItemWidget : public UUserWidget
{
	GENERATED_BODY()

	class UIngredientManagerSystem* IngredientManagerSystem;
	void NativePreConstruct();
	void NativeConstruct();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_Class;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_Item_Icon;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_Name;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_HavingNum;

	UFUNCTION(BlueprintImplementableEvent)
	void SetSellingAuctionPrice(int32 TableIdx);

	UFUNCTION(BlueprintImplementableEvent)
	void SetHavingNumText(int32 TableIdx);
	UFUNCTION(BlueprintCallable)
	int32 GetHavingIngrNum(int32 TableIdx);

	UFUNCTION(BlueprintCallable)
	void SetItemUI(int32 TableIdx);
};
