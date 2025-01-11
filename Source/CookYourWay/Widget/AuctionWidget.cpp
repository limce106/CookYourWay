// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/AuctionWidget.h"
#include <Blueprint/WidgetLayoutLibrary.h>

void UAuctionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ProgressBar_Auction = (UProgressBar*)GetWidgetFromName(TEXT("ProgressBar_Auction"));
	Image_SellingPriceBar = (UImage*)GetWidgetFromName(TEXT("Image_SellingPriceBar"));
	TextBlock_SellingPrice_Kor = (UTextBlock*)GetWidgetFromName(TEXT("TextBlock_SellingPrice_Kor"));
	TextBlock_SellPrice = (UTextBlock*)GetWidgetFromName(TEXT("TextBlock_SellPrice"));
	TextBlock_CurBidPrice = (UTextBlock*)GetWidgetFromName(TEXT("TextBlock_CurBidPrice"));
}

FReply UAuctionWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (AuctionSequence != PlayerTurn) {
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
	if (AuctionSequence != PlayerTurn) {
		return;
	}

	if (IsValid(BP_BidBar) && BP_BidBar != nullptr && !BP_BidBar->IsHovered()) {
		BP_BidBar->RemoveFromParent();
		BP_BidBar = nullptr;
	}
}

FReply UAuctionWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (AuctionSequence != PlayerTurn) {
		return FReply::Handled();
	}

	// 테스트를 위해 임시로 주석 처리
	/*if (!CanBuy) {
		return FReply::Handled();
	}*/

	IsPlayerBidPrevTurn = true;

	// 프로그래스바 퍼센트 설정
	FillProgressBarClickedPoint();

	// 현재까지 입찰된 가격 설정
	SetBidPriceTextByClickedPrice();
	SetBidPriceTextPosByPercent();

	return FReply::Handled();
}

FVector2D UAuctionWidget::GetCurLocalMousePos(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FVector2D LocalMousePos = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	return LocalMousePos;
}

float UAuctionWidget::PosToProgressBarPercent(FVector2D Pos)
{
	FVector2D ProgressBarSize = GetProgressBarSize();
	FVector2D ProgressBarPos = GetProgressBarPos();

	float Percent = FMath::Clamp((Pos.X - ProgressBarPos.X) / ProgressBarSize.X, 0.0f, 1.0f);
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

void UAuctionWidget::SetSellingPricePos(float SellingPrice)
{
	float Percent;
	PriceToProgressBarPercent(SellingPrice, Percent);

	FVector2D ProgressBarPos = GetProgressBarPos();
	FVector2D ProgressBarSize = GetProgressBarSize();
	float SellingPricePosX = ProgressBarPos.X + ProgressBarSize.X * Percent;

	UCanvasPanelSlot* ImageSellingPriceBarCanvasSlot = Cast<UCanvasPanelSlot>(Image_SellingPriceBar->Slot);
	ImageSellingPriceBarCanvasSlot->SetDesiredPosition(FVector2D(SellingPricePosX, 800.0f));

	UCanvasPanelSlot* TextSellingPriceKorCanvasSlot = Cast<UCanvasPanelSlot>(TextBlock_SellingPrice_Kor->Slot);
	TextSellingPriceKorCanvasSlot->SetDesiredPosition(FVector2D(SellingPricePosX, 920.0f));

	UCanvasPanelSlot* TextSellingPriceCanvasSlot = Cast<UCanvasPanelSlot>(TextBlock_SellPrice->Slot);
	TextSellingPriceCanvasSlot->SetDesiredPosition(FVector2D(SellingPricePosX, 954.5f));
}

void UAuctionWidget::SetBidPriceTextPosByPercent()
{
	float CurProgressBarPercent = ProgressBar_Auction->Percent;
	if (CurProgressBarPercent == 0.0f) {
		TextBlock_CurBidPrice->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (TextBlock_CurBidPrice->GetText().ToString() == TextBlock_SellPrice->GetText().ToString()) {
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
	// float SizeX = BP_Turn->GetDesiredSize().X;
	BP_Turn->SetPositionInViewport(FVector2D(PosX, 693.0f), false);
}

//float UAuctionWidget::PriceToProgressBarPercent(float Price, float BinMin, float BinMax)
//{
//	float Percent = (Price - BinMin) / (BinMax - BinMin);
//	return FMath::Clamp(Percent, 0.0f, 1.0f);;
//}
