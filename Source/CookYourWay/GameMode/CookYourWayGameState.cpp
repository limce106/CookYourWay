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

	static ConstructorHelpers::FClassFinder<AActor> BP_Store(TEXT("/Game/Blueprint/Village/BP_Store"));
	BP_StoreClass = BP_Store.Class;
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
	VillageManagerSystem->LoadedStoreData = CookYourWaySaveGame->StoreDatas;
	VillageManagerSystem->TotalAsset = CookYourWaySaveGame->TotalAsset;
	VillageManagerSystem->PlayerBistroTotalCust = CookYourWaySaveGame->PlayerBistroTotalCust;
	VillageManagerSystem->CompetitorTotalCust = CookYourWaySaveGame->CompetitorTotalCust;
	VillageManagerSystem->PlayerBistroTotalRateSum = CookYourWaySaveGame->PlayerBistroTotalRateSum;
	VillageManagerSystem->CompetitorTotalRateSum = CookYourWaySaveGame->CompetitorTotalRateSum;
	CustomerDataManagerSystem->IsRegularCustMap = CookYourWaySaveGame->IsRegularCustMap;
	CustomerDataManagerSystem->LoyaltyMap = CookYourWaySaveGame->LoyaltyMap;
	CustomerDataManagerSystem->AvgRateMap = CookYourWaySaveGame->AvgRateMap;
	CustomerDataManagerSystem->VisitedNumMap = CookYourWaySaveGame->VisitedNumMap;
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
	NewCookYourWayData->IsRegularCustMap = CustomerDataManagerSystem->IsRegularCustMap;
	NewCookYourWayData->LoyaltyMap = CustomerDataManagerSystem->LoyaltyMap;
	NewCookYourWayData->AvgRateMap = CustomerDataManagerSystem->AvgRateMap;
	NewCookYourWayData->VisitedNumMap = CustomerDataManagerSystem->VisitedNumMap;

	NewCookYourWayData->StoreDatas.Empty();
	//UClass* BP_StoreClass = StaticLoadClass(AStore::StaticClass(), nullptr, TEXT("/Game/Blueprint/Village/BP_Store.BP_Store_C"));

	TArray<AActor*> StoreActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BP_StoreClass, StoreActors);
	for (AActor* Actor : StoreActors)
	{
		AStore* Store = Cast<AStore>(Actor);
		NewCookYourWayData->StoreDatas.Add(Store->CurStoreData);
	}

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
