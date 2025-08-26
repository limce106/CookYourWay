// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CookYourWayGameMode.h"
#include <Kismet/GameplayStatics.h>
#include <GameInstance/CookYourWayGameInstance.h>
#include "GameFramework/GameUserSettings.h"
#include "Engine/GameViewportClient.h"

void ACookYourWayGameMode::InitGame(const FString& MapName, const FString& Option, FString& ErrorMessage)
{
	Super::InitGame(MapName, Option, ErrorMessage);

	VillageManagerSystem = UCookYourWayGameInstance::GetVillageManagerSystemStatic(this);
	CustomerDataManagerSystem = UCookYourWayGameInstance::GetCustomerDataManagerSystemStatic(this);
	IngredientManagerSystem = UCookYourWayGameInstance::GetIngredientManagerSystemStatic(this);
}

void ACookYourWayGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (Settings)
	{
		// 창모드, 해상도 고정
		Settings->SetFullscreenMode(EWindowMode::Windowed);
		Settings->SetScreenResolution(FIntPoint(1920, 1080));
		Settings->ApplySettings(false);
	}
}
