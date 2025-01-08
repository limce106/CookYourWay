// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/AuctionWidget.h"
#include <Blueprint/WidgetLayoutLibrary.h>

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
	bool IsMouseOnUnfilled = IsMouseOnUnfilledProgressBar(InGeometry, InMouseEvent);

	if (IsMouseOnUnfilled) {
		if (BP_BidBar == nullptr || !IsValid(BP_BidBar)) {
			CreateBidBar();
		}
		SetCurViewingBidPrice(GetProgressBarPercentOnMouse(InGeometry, InMouseEvent));

		FVector2D BidBarPos;
		UCanvasPanelSlot* ProgressBarCanvasSlot = Cast<UCanvasPanelSlot>(ProgressBar_Auction->Slot);
		FVector2D ProgressBarPos = ProgressBarCanvasSlot->GetPosition();
		FVector2D MouseScreenPos = InMouseEvent.GetScreenSpacePosition();

		BidBarPos.X = InGeometry.AbsoluteToLocal(MouseScreenPos).X;
		BidBarPos.Y = ProgressBarPos.Y - 12.0f;

		BP_BidBar->SetPositionInViewport(BidBarPos, false);
	}

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UAuctionWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (IsValid(BP_BidBar)) {
		BP_BidBar->RemoveFromParent();
		BP_BidBar = nullptr;
	}
}

FReply UAuctionWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 프로그래스바 퍼센트 설정
	float ClickedPercent = GetProgressBarPercentOnMouse(InGeometry, InMouseEvent);
	ProgressBar_Auction->SetPercent(ClickedPercent);

	// 현재까지 입찰된 가격 설정
	SetCurBidPrice();
	SetCurBidPricePos();

	return FReply::Handled();
}

FVector2D UAuctionWidget::GetCurLocalMousePos(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FVector2D LocalMousePos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	return LocalMousePos;
}

float UAuctionWidget::GetProgressBarPercentOnMouse(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FVector2D CurMousePos = GetCurLocalMousePos(InGeometry, InMouseEvent);
	FVector2D ProgressBarSize = InGeometry.GetLocalSize();

	float Percent = FMath::Clamp(CurMousePos.X / ProgressBarSize.X, 0.0f, 1.0f);
	return Percent;
}

bool UAuctionWidget::IsMouseOnUnfilledProgressBar(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FVector2D CurMousePos = GetCurLocalMousePos(InGeometry, InMouseEvent);

	float ProgressBarWidth = InGeometry.GetLocalSize().X;
	float FilledWidth = ProgressBarWidth * ProgressBar_Auction->Percent;

	// 마우스가 채워지지 않은 영역에 있는지
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
	ImageSellingPriceBarCanvasSlot->SetPosition(FVector2D(SellingPricePosX, 800.0f));

	UCanvasPanelSlot* TextSellingPriceKorCanvasSlot = Cast<UCanvasPanelSlot>(TextBlock_SellingPrice_Kor->Slot);
	TextSellingPriceKorCanvasSlot->SetPosition(FVector2D(SellingPricePosX, 920.0f));

	UCanvasPanelSlot* TextSellingPriceCanvasSlot = Cast<UCanvasPanelSlot>(TextBlock_SellingPrice->Slot);
	TextSellingPriceCanvasSlot->SetPosition(FVector2D(SellingPricePosX, 954.5f));
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

		FVector2D ProgressBarPos = GetProgressBarPos();
		FVector2D ProgressBarSize = GetProgressBarSize();

		float CurBidPricePosX = ProgressBarPos.X + ProgressBarSize.X * CurProgressBarPercent;

		UCanvasPanelSlot* CurBidPriceCanvasSlot = Cast<UCanvasPanelSlot>(TextBlock_CurBidPrice->Slot);
		CurBidPriceCanvasSlot->SetPosition(FVector2D(CurBidPricePosX, 920.0f));
	}
}
