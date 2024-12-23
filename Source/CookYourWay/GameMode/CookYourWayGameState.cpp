// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CookYourWayGameState.h"
#include <Kismet/GameplayStatics.h>

void ACookYourWayGameState::HandleBeginPlay() {
	Super::HandleBeginPlay();

	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();

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
}

void ACookYourWayGameState::SaveCookYourWayData()
{
	UCookYourWaySaveGame* NewCookYourWayData = NewObject<UCookYourWaySaveGame>();

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
