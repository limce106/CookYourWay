// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewsWidget.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API UNewsWidget : public UUserWidget
{
	GENERATED_BODY()
	class UIngredientManagerSystem* IngredientManagerSystem;
	class UCustomerDataManagerSystem* CustomerDataManagerSystem;
	class UVillageManagerSystem* VillageManagerSystem;

	bool ContinueIngrSeasonDay = false;
	
	void NativeConstruct();
	virtual void NativeTick(const FGeometry& Geometry, float DeltaSeconds) override;

	FString GetKeyWordByNum(FString Num);
	FString RedefineNewsString(FString News);
	FString GetRandomOriginalNewsStr();

	void SetNewsImg(FString Num, FString CurNewsKeyWord);

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class URichTextBlock* RichTextBlock_News;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* Image_News;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UBackgroundBlur* BackgroundBlur_CustBlur;

	UPROPERTY(BlueprintReadOnly)
	bool IsCustBlur = false;

	UFUNCTION()
	FString GetRedefinedNewsString();
	UFUNCTION()
	FString GetSeasonNewsNextString();
	UFUNCTION()
	bool IsContinueIngrSeasonDay();
	UFUNCTION()
	int32 GetYesterDayNewsIdx();
};
