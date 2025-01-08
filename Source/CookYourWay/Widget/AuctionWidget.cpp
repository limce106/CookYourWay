// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/AuctionWidget.h"
#include <Blueprint/WidgetLayoutLibrary.h>

#define PlayerTurn 0;
#define CompetitorTurn 1;

void UAuctionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ProgressBar_Auction = (UProgressBar*)GetWidgetFromName(TEXT("ProgressBar_Auction"));
	Image_SellingPriceBar = (UImage*)GetWidgetFromName(TEXT("Image_SellingPriceBar"));
	TextBlock_SellingPrice_Kor = (UTextBlock*)GetWidgetFromName(TEXT("TextBlock_SellingPrice_Kor"));
	TextBlock_SellingPrice = (UTextBlock*)GetWidgetFromName(TEXT("TextBlock_SellingPrice"));
	TextBlock_CurBidPrice = (UTextBlock*)GetWidgetFromName(TEXT("TextBlock_CurBidPrice"));
}

FReply UAuctionWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (AuctionSequence != 0) {
		return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
	}

	bool IsMouseOnUnfilled = IsMouseOnUnfilledProgressBar(InGeometry, InMouseEvent);

	if (IsMouseOnUnfilled) {
		if (BP_BidBar == nullptr || !IsValid(BP_BidBar)) {
			CreateBidBar();
		}
	}

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UAuctionWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (AuctionSequence != 0) {
		return;
	}

	if (IsValid(BP_BidBar)) {
		BP_BidBar->RemoveFromParent();
		BP_BidBar = nullptr;
	}
}

FReply UAuctionWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (AuctionSequence != 0) {
		return FReply::Handled();
	}

	// ���α׷����� �ۼ�Ʈ ����
	FillProgressBarClickedPoint();

	// ������� ������ ���� ����
	SetCurBidPrice();
	SetCurBidPricePos();

	return FReply::Handled();
}

FVector2D UAuctionWidget::GetCurLocalMousePos(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FVector2D LocalMousePos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	return LocalMousePos;
}

float UAuctionWidget::BidBarPosToProgressBarPercent(FVector2D BidBarPos)
{
	FVector2D ProgressBarSize = GetProgressBarSize();

	float Percent = FMath::Clamp(BidBarPos.X / ProgressBarSize.X, 0.0f, 1.0f);
	return Percent;
}

bool UAuctionWidget::IsMouseOnUnfilledProgressBar(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FVector2D CurMousePos = GetCurLocalMousePos(InGeometry, InMouseEvent);

	float ProgressBarWidth = InGeometry.GetLocalSize().X;
	float FilledWidth = ProgressBarWidth * ProgressBar_Auction->Percent;

	// ���콺�� ä������ ���� ������ �ִ���
	if (CurMousePos.X > FilledWidth) {
		return true;
	}
	else {
		return false;
	}
}

FVector2D UAuctionWidget::GetProgressBarSize()
{
	UCanvasPanelSlot* ProgressBarCanvasSlot = Cast<UCanvasPanelSlot>(ProgressBar_Auction->Slot);
	FVector2D ProgressBarSize = ProgressBarCanvasSlot->GetSize();

	return ProgressBarSize;
}

FVector2D UAuctionWidget::GetProgressBarPos()
{
	UCanvasPanelSlot* ProgressBarCanvasSlot = Cast<UCanvasPanelSlot>(ProgressBar_Auction->Slot);
	FVector2D ProgressBarPos = ProgressBarCanvasSlot->GetPosition();

	return ProgressBarPos;
}

void UAuctionWidget::SetSellingPricePos(float BinMin, float SellingPrice, float BinMax)
{
	float Percent = (SellingPrice - BinMin) / (BinMax - BinMin);

	FVector2D ProgressBarPos = GetProgressBarPos();
	FVector2D ProgressBarSize = GetProgressBarSize();
	float SellingPricePosX = ProgressBarPos.X + ProgressBarSize.X * Percent;

	UCanvasPanelSlot* ImageSellingPriceBarCanvasSlot = Cast<UCanvasPanelSlot>(Image_SellingPriceBar->Slot);
	ImageSellingPriceBarCanvasSlot->SetDesiredPosition(FVector2D(SellingPricePosX, 800.0f));

	UCanvasPanelSlot* TextSellingPriceKorCanvasSlot = Cast<UCanvasPanelSlot>(TextBlock_SellingPrice_Kor->Slot);
	TextSellingPriceKorCanvasSlot->SetDesiredPosition(FVector2D(SellingPricePosX, 920.0f));

	UCanvasPanelSlot* TextSellingPriceCanvasSlot = Cast<UCanvasPanelSlot>(TextBlock_SellingPrice->Slot);
	TextSellingPriceCanvasSlot->SetDesiredPosition(FVector2D(SellingPricePosX, 954.5f));
}

void UAuctionWidget::SetCurBidPricePos()
{
	float CurProgressBarPercent = ProgressBar_Auction->Percent;
	if (CurProgressBarPercent == 0.0f) {
		TextBlock_CurBidPrice->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (TextBlock_CurBidPrice->GetText().ToString() == TextBlock_SellingPrice->GetText().ToString()) {
		TextBlock_CurBidPrice->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		TextBlock_CurBidPrice->SetVisibility(ESlateVisibility::Visible);

		float CurBidPricePosX = GetFilledProgressBarPosX();
		UCanvasPanelSlot* CurBidPriceCanvasSlot = Cast<UCanvasPanelSlot>(TextBlock_CurBidPrice->Slot);
		float CurBidPriceSizeX = CurBidPriceCanvasSlot->GetSize().X;
		CurBidPricePosX -= CurBidPriceSizeX / 2;
		CurBidPriceCanvasSlot->SetPosition(FVector2D(CurBidPricePosX, 920.0f));
	}
}

float UAuctionWidget::GetFilledProgressBarPosX()
{
	FVector2D ProgressBarPos = GetProgressBarPos();
	FVector2D ProgressBarSize = GetProgressBarSize();
	float Percent = ProgressBar_Auction->Percent;

	float FilledPosX = ProgressBarPos.X + ProgressBarSize.X * Percent;
	return FilledPosX;
}

void UAuctionWidget::SetTurnWidgetPos()
{
	float PosX = GetFilledProgressBarPosX();
	BP_Turn->SetPositionInViewport(FVector2D(PosX, 693.0f), false);
}

void UAuctionWidget::StartAuction()
{
	FLinearColor CurColor;
	if (AuctionSequence == 0) {
		CurColor = FLinearColor(1.0f, 0.617207f, 0.078187f, 1.0f);
	}
	else if (AuctionSequence == 1) {
		CurColor = FLinearColor(0.768151f, 0.059511f, 0.059511f, 1.0f);
	}
	SetTurnWidgetPos();
}
