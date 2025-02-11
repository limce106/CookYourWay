// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameInstance/VillageManagerSystem.h"
#include "GameInstance/CustomerDataManagerSystem.h"
#include "GameMode/CookYourWayGameState.h"
#include "Actor/Store.h"
#include "VillageManager.generated.h"

UCLASS()
class COOKYOURWAY_API AVillageManager : public AActor
{
	GENERATED_BODY()

	class APlayerBistro* PlayerBistro;

	FTimerHandle LeftDayTimeHandler;
	int32 LeftMinute = 0;
	int32 LeftSecond = 3;

	// 남은 하루 시간 계산하기
	void DecreaseDayTime();

	void Init();
	void RunDayTimer();

	void SpawnBistrosAndStore();
	void DecreaseStorePeriod();

	void TryCreateNewCompetitor();
	void TryCreateNewStore();
	void CreateNewStore();
	void AddRandomRegularCust(int32 AreaID, int32 RegularCustNum);

	void EndEatingCustomer();
	void SaveStoreDataInManager();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_PlayerBistro;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Competitor;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AStore> BP_Store;

public:
	AVillageManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	class UVillageManagerSystem* VillageManagerSystem;
	class UCustomerDataManagerSystem* CustomerDataManagerSystem;
	ACookYourWayGameState* CookYourWayGameState;

	TMap<int32, FVector> AreaLocMap;

	UPROPERTY()
	TArray<FCustomerCommentData> RedefinedCustomerCommentTableRows;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<bool>IsCommentTalked;

	UFUNCTION(BlueprintCallable)
	int32 GetLeftMinute();
	UFUNCTION(BlueprintCallable)
	int32 GetLeftSecond();

	int32 GetRandomAreaId();

	// 하루가 끝났을 때
	void EndDay();

	// 날 수를 주차, 요일 문자열로 변환
	UFUNCTION(BlueprintCallable)
	FString DayToWeekString(int32 Day);

	// 페이드 아웃 후 정산표 띄우기
	UFUNCTION(BlueprintImplementableEvent)
	void StartFadeOutAnim();
	UFUNCTION(BlueprintImplementableEvent)
	void StartSubtractAnim();

	void UpdateProfitsValue(int32 Value);
};
