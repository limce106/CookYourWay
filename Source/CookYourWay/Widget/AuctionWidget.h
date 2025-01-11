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
	// 직전 플레이어 턴에서 입찰했는지 여부
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

	// 판매가 텍스트 위치 설정
	UFUNCTION(BlueprintCallable)
	void SetSellingPricePos(float SellingPrice);
	// 프로그래스바 퍼센트에 따라 입찰 가격 텍스트 위치 설정
	UFUNCTION(BlueprintCallable)
	void SetBidPriceTextPosByPercent();
	// 프로그래스바 채워진 부분의 X 위치 가져오기
	UFUNCTION(BlueprintCallable)
	float GetFilledProgressBarPosX();
	// 턴 타이머 위젯 위치 설정
	UFUNCTION(BlueprintCallable)
	void SetTurnWidgetPos();

	// 입찰 바 생성
	UFUNCTION(BlueprintImplementableEvent)
	void CreateBidBar();
	// 프로그래스바 퍼선트에 따라 입찰 바 가격 텍스트 설정
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetBidBarPrice(float Percent);
	// 클릭한 가격에 따라 입찰 바 가격 텍스트 설정
	UFUNCTION(BlueprintImplementableEvent)
	void SetBidPriceTextByClickedPrice();
	// 클리한 지점까지 프로그래스바 채우기
	UFUNCTION(BlueprintImplementableEvent)
	void FillProgressBarClickedPoint();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PercentToPrice(float Percent, int32& Price);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PosToPrice(FVector2D Pos, int32& Price);
	// 가격을 프로그래스바 퍼센트로 변환
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PriceToProgressBarPercent(float Price, float& Percent);
};
