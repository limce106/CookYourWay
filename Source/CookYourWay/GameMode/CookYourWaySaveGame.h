// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include <GameInstance/CustomerDataManagerSystem.h>
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
	TArray<int32> CompetitorAreaID = { 5, 7, 11, 14, 18, 21 };
	UPROPERTY()
	TArray<int32> StoreAreaID = { 2, 10, 23 };
	UPROPERTY()
	int32 TotalAsset = 0;
	UPROPERTY()
	TArray<FString> CustomerNames;
	UPROPERTY()
	TMap<FCustomerBistroKey, bool> IsRegularCustMap;
	UPROPERTY()
	TMap<FCustomerBistroKey, float> LoyaltyMap;
	UPROPERTY()
	TMap<FCustomerBistroKey, float> AvgRateMap;
};
