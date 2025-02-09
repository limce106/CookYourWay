// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CookYourWayGameMode.h"
#include <Kismet/GameplayStatics.h>

void ACookYourWayGameMode::InitGame(const FString& MapName, const FString& Option, FString& ErrorMessage)
{
	Super::InitGame(MapName, Option, ErrorMessage);

	VillageManagerSystem = GetGameInstance()->GetSubsystem<UVillageManagerSystem>();
	IngredientManagerSystem = GetGameInstance()->GetSubsystem<UIngredientManagerSystem>();
	CustomerDataManagerSystem = GetGameInstance()->GetSubsystem<UCustomerDataManagerSystem>();

	VillageManagerSystem->Day++;
}

void ACookYourWayGameMode::BeginPlay()
{
	Super::BeginPlay();
}
