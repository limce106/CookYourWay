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
	
	void NativeConstruct();
	virtual void NativeTick(const FGeometry& Geometry, float DeltaSeconds) override;

	FString GetKeyWordByNum(int32 Num);
	FString RedefineNewsString(FString News);
	FString GetRandomOriginalNewsStr();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class URichTextBlock* RichTextBlock_News;

	UFUNCTION(BlueprintCallable)
	FString GetRedefinedNewsString();

};
