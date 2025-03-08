// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/AuctionWidget.h"
#include <Blueprint/WidgetLayoutLibrary.h>
#include <Kismet/GameplayStatics.h>

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

	bool IsMouseOnUnfilled = IsMouseOnUnfilledProgressBar();

	bool Equal;
	HoverPriceEqualCurBid(Equal);

	if (IsMouseOnUnfilled && !IsPlayerBidThisTurn && !Equal) {
		if (BP_BidBar == nullptr || !IsValid(BP_BidBar)) {
			CreateBidBar();
		}
		else {
			if (BP_BidBar->GetVisibility() == ESlateVisibility::Hidden) {
				BP_BidBar->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
	}
	else {
		if (BP_BidBar) {
			BP_BidBar->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UAuctionWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (AuctionSequence != PlayerTurn) {
		return;
	}

	if (IsValid(BP_BidBar) && BP_BidBar->GetVisibility() != ESlateVisibility::Hidden) {
		BP_BidBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

FReply UAuctionWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (AuctionSequence != PlayerTurn) {
		return FReply::Handled();
	}

	if (!CanBuy) {
		return FReply::Handled();
	}

	if (IsValid(BP_BidBar) && BP_BidBar != nullptr) {
		BP_BidBar->RemoveFromParent();
	}
	IsPlayerBidThisTurn = true;

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
	if (!IsValid(ProgressBar_Auction)) {
		return 0.0f;
	}

	FVector2D ProgressBarSize = GetProgressBarSize();
	FVector2D ProgressBarPos = GetProgressBarPos();

	float Percent = FMath::Clamp((Pos.X - ProgressBarPos.X) / ProgressBarSize.X, 0.0f, 1.0f);
	return Percent;
}

bool UAuctionWidget::IsMouseOnUnfilledProgressBar()
{
	float MouseX, MouseY;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->GetMousePosition(MouseX, MouseY);

	float ProgressBarY = GetProgressBarPos().Y;
	float ProgressBarHeight = GetProgressBarSize().Y;

	// 마우스가 채워지지 않은 영역에 있는지
	if (MouseX > GetFilledProgressBarPosX() && MouseY >= ProgressBarY && MouseY <= (ProgressBarY + ProgressBarHeight)) {
		return true;
	}
	else {
		return false;
	}
}

FVector2D UAuctionWidget::GetProgressBarSize()
{
	if (!IsValid(ProgressBar_Auction)) {
		return FVector2D(0.0f, 0.0f);
	}

	UCanvasPanelSlot* ProgressBarCanvasSlot = Cast<UCanvasPanelSlot>(ProgressBar_Auction->Slot);
	FVector2D ProgressBarSize = ProgressBarCanvasSlot->GetSize();

	return ProgressBarSize;
}

FVector2D UAuctionWidget::GetProgressBarPos()
{
	if (!IsValid(ProgressBar_Auction)) {
		return FVector2D(0.0f, 0.0f);
	}

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
	ImageSellingPriceBarCanvasSlot->SetPosition(FVector2D(SellingPricePosX - (ImageSellingPriceBarCanvasSlot->GetSize().X / 2), 800.0f));

	UCanvasPanelSlot* TextSellingPriceKorCanvasSlot = Cast<UCanvasPanelSlot>(TextBlock_SellingPrice_Kor->Slot);
	TextSellingPriceKorCanvasSlot->SetPosition(FVector2D(SellingPricePosX - (TextSellingPriceKorCanvasSlot->GetSize().X / 2), 920.0f));

	UCanvasPanelSlot* TextSellingPriceCanvasSlot = Cast<UCanvasPanelSlot>(TextBlock_SellPrice->Slot);
	TextSellingPriceCanvasSlot->SetPosition(FVector2D(SellingPricePosX - (TextSellingPriceCanvasSlot->GetSize().X / 2), 954.5f));
}

void UAuctionWidget::SetBidPriceTextPosByPercent()
{
	float CurProgressBarPercent = ProgressBar_Auction->GetPercent();
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
	float Percent = ProgressBar_Auction->GetPercent();

	float FilledPosX = ProgressBarPos.X + ProgressBarSize.X * Percent;
	return FilledPosX;
}

void UAuctionWidget::SetTurnWidgetPos()
{
	float PosX = GetFilledProgressBarPosX();
	BP_Turn->SetPositionInViewport(FVector2D(PosX, 693.0f), false);
}
