// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "AuctionWidget.generated.h"

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
	class UTextBlock* TextBlock_SellPrice;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TextBlock_CurBidPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* BP_BidBar = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* BP_Turn;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 AuctionSequence = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PlayerTurn = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CompetitorTurn = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CanBuy = false;
	// ���� �÷��̾� �Ͽ��� �����ߴ��� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsPlayerBidPrevTurn = false;

	UFUNCTION()
	FVector2D GetCurLocalMousePos(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	UFUNCTION(BlueprintCallable)
	float PosToProgressBarPercent(FVector2D Pos);
	UFUNCTION()
	bool IsMouseOnUnfilledProgressBar(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	UFUNCTION()
	FVector2D GetProgressBarSize();
	UFUNCTION()
	FVector2D GetProgressBarPos();

	// �ǸŰ� �ؽ�Ʈ ��ġ ����
	UFUNCTION(BlueprintCallable)
	void SetSellingPricePos(float SellingPrice);
	// ���α׷����� �ۼ�Ʈ�� ���� ���� ���� �ؽ�Ʈ ��ġ ����
	UFUNCTION(BlueprintCallable)
	void SetBidPriceTextPosByPercent();
	// ���α׷����� ä���� �κ��� X ��ġ ��������
	UFUNCTION(BlueprintCallable)
	float GetFilledProgressBarPosX();
	// �� Ÿ�̸� ���� ��ġ ����
	UFUNCTION(BlueprintCallable)
	void SetTurnWidgetPos();

	// ���� �� ����
	UFUNCTION(BlueprintImplementableEvent)
	void CreateBidBar();
	// ���α׷����� �ۼ�Ʈ�� ���� ���� �� ���� �ؽ�Ʈ ����
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetBidBarPrice(float Percent);
	// Ŭ���� ���ݿ� ���� ���� �� ���� �ؽ�Ʈ ����
	UFUNCTION(BlueprintImplementableEvent)
	void SetBidPriceTextByClickedPrice();
	// Ŭ���� �������� ���α׷����� ä���
	UFUNCTION(BlueprintImplementableEvent)
	void FillProgressBarClickedPoint();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PercentToPrice(float Percent, int32& Price);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PosToPrice(FVector2D Pos, int32& Price);
	// ������ ���α׷����� �ۼ�Ʈ�� ��ȯ
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PriceToProgressBarPercent(float Price, float& Percent);
};
