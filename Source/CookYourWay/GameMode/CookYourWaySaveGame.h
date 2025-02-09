// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include <GameInstance/CustomerDataManagerSystem.h>
#include <GameInstance/VillageManagerSystem.h>
#include "CookYourWaySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API UCookYourWaySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UCookYourWaySaveGame();

	UPROPERTY()
	TArray<int32> HavingIngrNum;
	UPROPERTY()
	int32 Day = 0;
	UPROPERTY()
	int32 PlayerBistroAreaID = 16;
	UPROPERTY()
	TArray<FCompetitorData> CompetitorDataArr;
	UPROPERTY()
	TArray<FPlayerBistroRatingData> PlayerBistroRatingDataArr;
	UPROPERTY()
	TArray<FStoreData> StoreDataArr;
	UPROPERTY()
	int32 TotalAsset = 0;
	UPROPERTY()
	TMap<FCustomerBistroKey, bool> IsRegularCustMap;
	UPROPERTY()
	TMap<FCustomerBistroKey, int32> LoyaltyMap;
	UPROPERTY()
	TMap<FCustomerBistroKey, int32> MaxSatisfactionMap;
	UPROPERTY()
	int32 PlayerBistroTotalCust = 0;
	UPROPERTY()
	int32 PlayerBistroRating = 0;
	UPROPERTY()
	int32 DestroyedCompetitor = 0;
	UPROPERTY()
	FString PlayerBistroName = "매장 이름";
};
