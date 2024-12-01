// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VillageManager.generated.h"

UCLASS()
class COOKYOURWAY_API AVillageManager : public AActor
{
	GENERATED_BODY()

	TMap<int32, FVector> AreaLocMap;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_PlayerBistro;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Competitor;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Store;

	void Init();

public:
	AVillageManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	TArray<FString> CustomerNames = {
		"Amy",
		"Michelle"
	};

	FString GetRandomCustName();
};
