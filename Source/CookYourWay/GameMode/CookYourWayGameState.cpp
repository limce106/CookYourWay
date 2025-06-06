// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CookYourWayGameState.h"
#include <Kismet/GameplayStatics.h>
#include <Actor/Store.h>
#include <GameInstance/CookYourWayGameInstance.h>

void ACookYourWayGameState::BeginPlay()
{
	VillageManagerSystem = UCookYourWayGameInstance::GetVillageManagerSystemStatic(this);
	CustomerDataManagerSystem = UCookYourWayGameInstance::GetCustomerDataManagerSystemStatic(this);
	IngredientManagerSystem = UCookYourWayGameInstance::GetIngredientManagerSystemStatic(this);
}

ACookYourWayGameState::ACookYourWayGameState()
{
	SaveSlotName = TEXT("CookYourWaySaveFile");
	UserIndex = 0;
}

void ACookYourWayGameState::LoadCookYourWayData()
{
	UCookYourWaySaveGame* CookYourWaySaveGame = Cast<UCookYourWaySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));
	if (CookYourWaySaveGame == nullptr) {
		CookYourWaySaveGame = GetMutableDefault<UCookYourWaySaveGame>();
	}

	IngredientManagerSystem->HavingIngrNum = CookYourWaySaveGame->HavingIngrNum;
	VillageManagerSystem->Day = CookYourWaySaveGame->Day;
	VillageManagerSystem->PlayerBistroAreaID = CookYourWaySaveGame->PlayerBistroAreaID;
	VillageManagerSystem->CompetitorDataArr = CookYourWaySaveGame->CompetitorDataArr;
	CustomerDataManagerSystem->PlayerBistroRatingDataArr = CookYourWaySaveGame->PlayerBistroRatingDataArr;
	VillageManagerSystem->StoreDataArr = CookYourWaySaveGame->StoreDataArr;
	VillageManagerSystem->TotalAsset = CookYourWaySaveGame->TotalAsset;
	VillageManagerSystem->PlayerBistroTotalCust = CookYourWaySaveGame->PlayerBistroTotalCust;
	VillageManagerSystem->PlayerBistroRating = CookYourWaySaveGame->PlayerBistroRating;
	CustomerDataManagerSystem->IsRegularCustMap = CookYourWaySaveGame->IsRegularCustMap;
	CustomerDataManagerSystem->LoyaltyMap = CookYourWaySaveGame->LoyaltyMap;
	CustomerDataManagerSystem->MaxSatisfactionMap = CookYourWaySaveGame->MaxSatisfactionMap;
	VillageManagerSystem->DestroyedCompetitor = CookYourWaySaveGame->DestroyedCompetitor;
	VillageManagerSystem->PlayerBistroName = CookYourWaySaveGame->PlayerBistroName;
}

void ACookYourWayGameState::SaveCookYourWayData()
{
	UCookYourWaySaveGame* NewCookYourWayData = NewObject<UCookYourWaySaveGame>();

	NewCookYourWayData->HavingIngrNum = IngredientManagerSystem->HavingIngrNum;
	NewCookYourWayData->Day = VillageManagerSystem->Day;
	NewCookYourWayData->PlayerBistroAreaID = VillageManagerSystem->PlayerBistroAreaID;
	NewCookYourWayData->CompetitorDataArr = VillageManagerSystem->CompetitorDataArr;
	NewCookYourWayData->PlayerBistroRatingDataArr = CustomerDataManagerSystem->PlayerBistroRatingDataArr;
	NewCookYourWayData->StoreDataArr = VillageManagerSystem->StoreDataArr;
	NewCookYourWayData->TotalAsset = VillageManagerSystem->TotalAsset;
	NewCookYourWayData->PlayerBistroTotalCust = VillageManagerSystem->PlayerBistroTotalCust;
	NewCookYourWayData->PlayerBistroRating = VillageManagerSystem->PlayerBistroRating;
	NewCookYourWayData->IsRegularCustMap = CustomerDataManagerSystem->IsRegularCustMap;
	NewCookYourWayData->LoyaltyMap = CustomerDataManagerSystem->LoyaltyMap;
	NewCookYourWayData->MaxSatisfactionMap = CustomerDataManagerSystem->MaxSatisfactionMap;
	NewCookYourWayData->DestroyedCompetitor = VillageManagerSystem->DestroyedCompetitor;
	NewCookYourWayData->PlayerBistroName = VillageManagerSystem->PlayerBistroName;

	if (!UGameplayStatics::SaveGameToSlot(NewCookYourWayData, SaveSlotName, UserIndex)) {
		UE_LOG(LogTemp, Error, TEXT("SaveGame Error!"));
	}
}

bool ACookYourWayGameState::IsSaveDataExist()
{
	UCookYourWaySaveGame* CookYourWaySaveGame = Cast<UCookYourWaySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));
	if (CookYourWaySaveGame == nullptr) {
		return false;
	}
	else {
		return true;
	}
}

void ACookYourWayGameState::DeleteCookYourWayData()
{
	UCookYourWaySaveGame* CookYourWaySaveGame = Cast<UCookYourWaySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));
	if (CookYourWaySaveGame) {
		UGameplayStatics::DeleteGameInSlot(SaveSlotName, UserIndex);
	}
}
