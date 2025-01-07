// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "AuctionWidget.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API UAuctionWidget : public UUserWidget
{
	GENERATED_BODY()
	
	void NativeConstruct();
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* ProgressBar_Auction;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_SellingPriceBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_SellingPrice_Kor;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_SellingPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* BP_BidBar;

	UFUNCTION()
	FVector2D GetCurLocalMousePos(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	UFUNCTION()
	float GetProgressBarPercentOnMouse(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	UFUNCTION()
	bool IsMouseOnUnfilledProgressBar(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	UFUNCTION()
	FVector2D GetProgressBarSize();
	UFUNCTION()
	FVector2D GetProgressBarPos();

	UFUNCTION(BlueprintCallable)
	void SetSellingPriceBarPos(float BinMin, float SellingPrice, float BinMax);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateBidBar();
	UFUNCTION(BlueprintImplementableEvent)
	void SetCurViewingBidPrice(float PercentOnMouse);
};
