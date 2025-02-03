// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CookYourWayGameState.h"
#include <Kismet/GameplayStatics.h>
#include <Actor/Store.h>

void ACookYourWayGameState::BeginPlay()
{
	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();
	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();
	CustomerDataManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UCustomerDataManagerSystem>();
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
	VillageManagerSystem->DestroyedCompetitor = CookYourWaySaveGame->DestroyedCompetitor;
	CustomerDataManagerSystem->IsCommentTalked = CookYourWaySaveGame->IsCommentTalked;
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
	NewCookYourWayData->DestroyedCompetitor = VillageManagerSystem->DestroyedCompetitor;
	NewCookYourWayData->IsCommentTalked = CustomerDataManagerSystem->IsCommentTalked;

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
