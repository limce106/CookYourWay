// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CookYourWaySaveGame.h"
#include "GameInstance/VillageManagerSystem.h"
#include "CookYourWayGameState.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API ACookYourWayGameState : public AGameState
{
	GENERATED_BODY()
	ACookYourWayGameState();
	class UVillageManagerSystem* VillageManagerSystem;

public:
	UPROPERTY(Transient)
	FString SaveSlotName;
	UPROPERTY(Transient)
	int32 UserIndex = 0;

	UFUNCTION(BlueprintCallable)
	void LoadCookYourWayData();
	UFUNCTION(BlueprintCallable)
	void SaveCookYourWayData();
	UFUNCTION(BlueprintCallable)
	bool IsSaveDataExist();

protected:
	void HandleBeginPlay() override;
};
