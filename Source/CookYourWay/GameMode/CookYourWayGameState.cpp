// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CookYourWayGameState.h"
#include <Kismet/GameplayStatics.h>

void ACookYourWayGameState::HandleBeginPlay() {
	Super::HandleBeginPlay();

	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();
	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();
	CustomerDataManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UCustomerDataManagerSystem>();

	UE_LOG(LogTemp, Warning, TEXT("HandleBeginPlay called!"));
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
	VillageManagerSystem->CompetitorAreaID = CookYourWaySaveGame->CompetitorAreaID;
	VillageManagerSystem->StoreAreaID = CookYourWaySaveGame->StoreAreaID;
	VillageManagerSystem->TotalAsset = CookYourWaySaveGame->TotalAsset;
	VillageManagerSystem->PlayerBistroTotalCust = CookYourWaySaveGame->PlayerBistroTotalCust;
	VillageManagerSystem->CompetitorTotalCust = CookYourWaySaveGame->CompetitorTotalCust;
	VillageManagerSystem->PlayerBistroTotalRateSum = CookYourWaySaveGame->PlayerBistroTotalRateSum;
	VillageManagerSystem->CompetitorTotalRateSum = CookYourWaySaveGame->CompetitorTotalRateSum;
	CustomerDataManagerSystem->CustomerNames = CookYourWaySaveGame->CustomerNames;
	CustomerDataManagerSystem->IsRegularCustMap = CookYourWaySaveGame->IsRegularCustMap;
	CustomerDataManagerSystem->LoyaltyMap = CookYourWaySaveGame->LoyaltyMap;
	CustomerDataManagerSystem->AvgRateMap = CookYourWaySaveGame->AvgRateMap;
}

void ACookYourWayGameState::SaveCookYourWayData()
{
	UCookYourWaySaveGame* NewCookYourWayData = NewObject<UCookYourWaySaveGame>();

	NewCookYourWayData->HavingIngrNum = IngredientManagerSystem->HavingIngrNum;
	NewCookYourWayData->Day = VillageManagerSystem->Day;
	NewCookYourWayData->PlayerBistroAreaID = VillageManagerSystem->PlayerBistroAreaID;
	NewCookYourWayData->CompetitorAreaID = VillageManagerSystem->CompetitorAreaID;
	NewCookYourWayData->StoreAreaID = VillageManagerSystem->StoreAreaID;
	NewCookYourWayData->TotalAsset = VillageManagerSystem->TotalAsset;
	NewCookYourWayData->PlayerBistroTotalCust = VillageManagerSystem->PlayerBistroTotalCust;
	NewCookYourWayData->CompetitorTotalCust = VillageManagerSystem->CompetitorTotalCust;
	NewCookYourWayData->PlayerBistroTotalRateSum = VillageManagerSystem->PlayerBistroTotalRateSum;
	NewCookYourWayData->CompetitorTotalRateSum = VillageManagerSystem->CompetitorTotalRateSum;
	NewCookYourWayData->CustomerNames = CustomerDataManagerSystem->CustomerNames;
	NewCookYourWayData->IsRegularCustMap = CustomerDataManagerSystem->IsRegularCustMap;
	NewCookYourWayData->LoyaltyMap = CustomerDataManagerSystem->LoyaltyMap;
	NewCookYourWayData->AvgRateMap = CustomerDataManagerSystem->AvgRateMap;

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
