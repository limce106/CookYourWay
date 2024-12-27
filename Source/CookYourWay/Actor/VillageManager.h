// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameInstance/VillageManagerSystem.h"
#include "GameInstance/CustomerDataManagerSystem.h"
#include "GameMode/CookYourWayGameState.h"
#include "VillageManager.generated.h"

UCLASS()
class COOKYOURWAY_API AVillageManager : public AActor
{
	GENERATED_BODY()

	class UVillageManagerSystem* VillageManagerSystem;
	class UCustomerDataManagerSystem* CustomerDataManagerSystem;
	ACookYourWayGameState* CookYourWayGameState;

	FTimerHandle LeftDayTimeHandler;
	int32 LeftMinute = 0;
	int32 LeftSecond = 5;

	// 남은 하루 시간 계산하기
	void DecreaseDayTime();

	void Init();
	void RunDayTimer();

	void SpawnBistrosAndStore();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_PlayerBistro;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Competitor;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Store;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> BP_SubtractClass;

public:
	AVillageManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	TMap<int32, FVector> AreaLocMap;

	UFUNCTION(BlueprintCallable)
	int32 GetLeftMinute();
	UFUNCTION(BlueprintCallable)
	int32 GetLeftSecond();

	void TryCreateNewCompetitor();
	int32 GetRandomAreaId();

	// 하루가 끝났을 때
	void EndDay();

	// 날 수를 주차, 요일 문자열로 변환
	UFUNCTION(BlueprintCallable)
	FString DayToWeekString(int32 Day);
};
