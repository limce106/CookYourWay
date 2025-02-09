// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CompetitorBookWidget.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API UCompetitorBookWidget : public UUserWidget
{
	GENERATED_BODY()

	class UCustomerDataManagerSystem* CustomerDataManagerSystem;
	class UVillageManagerSystem* VillageManagerSystem;

	void NativeConstruct();

public:
	UFUNCTION(BlueprintCallable)
	TArray<FCompetitorRatingData> GetSortedArrByGreaterRating(TArray<FCompetitorRatingData> RatingArr);
};
