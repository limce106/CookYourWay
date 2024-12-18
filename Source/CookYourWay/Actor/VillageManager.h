// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VillageManager.generated.h"

UCLASS()
class COOKYOURWAY_API AVillageManager : public AActor
{
	GENERATED_BODY()

	class UIngredientManagerSystem* IngredientManagerSystem;

	TMap<int32, FVector> AreaLocMap;

	FTimerHandle LeftDayTimeHandler;
	int32 LeftMinute = 5;
	int32 LeftSecond = 0;

	// 남은 하루 시간 계산하기
	void DecreaseDayTime();

	void Init();
	void RunDayTimer();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_PlayerBistro;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Competitor;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Store;

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

	TMap<FString, TArray<int32>> CustNameToTasteMap;

	// 랜덤 손님 이름 가져오기
	FString GetRandomCustName();

	// 랜덤 취향 배열 반환
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetRandomTaste();
	// 모든 손님 유형의 취향 설정
	UFUNCTION(BlueprintCallable)
	void SetAllCustTastes();
	// 특정 손님의 취향 가져오기
	TArray<int32> GetCustTaste(FString CustName);

	UFUNCTION(BlueprintCallable)
	int32 GetLeftMinute();
	UFUNCTION(BlueprintCallable)
	int32 GetLeftSecond();
};
