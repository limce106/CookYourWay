// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CookYourWayGameMode.h"
#include <Kismet/GameplayStatics.h>
#include <GameInstance/CookYourWayGameInstance.h>

void ACookYourWayGameMode::InitGame(const FString& MapName, const FString& Option, FString& ErrorMessage)
{
	Super::InitGame(MapName, Option, ErrorMessage);

	VillageManagerSystem = UCookYourWayGameInstance::GetVillageManagerSystemStatic(this);
	CustomerDataManagerSystem = UCookYourWayGameInstance::GetCustomerDataManagerSystemStatic(this);
	IngredientManagerSystem = UCookYourWayGameInstance::GetIngredientManagerSystemStatic(this);

	// Å×½ºÆ®
	//VillageManagerSystem->Day++;
	//
}

void ACookYourWayGameMode::BeginPlay()
{
	Super::BeginPlay();
}
