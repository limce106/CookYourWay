// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VillageManagerSystem.generated.h"

USTRUCT(BlueprintType)
struct FCompetitorReviewData : public FTableRowBase {
	GENERATED_BODY()

public:
	FCompetitorReviewData() : RatingMin(-1), RatingMax(-1), RatingProb(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 RatingMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 RatingMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float RatingProb;
};

USTRUCT(BlueprintType)
struct FStoreData : public FTableRowBase {
	GENERATED_BODY()

public:
	FStoreData() : StoreName("-1"), StoreScope(-1), StorePeriod(-1), StoreCust1("-1"), StoreCust2("-1"), StoreCust3("-1"), 
					StoreCustCreateProb(0), StoreGroupCode(-1) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString StoreName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 StoreScope;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 StorePeriod;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString StoreCust1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString StoreCust2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString StoreCust3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float StoreCustCreateProb;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 StoreGroupCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D* StoreIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UStaticMesh* StoreModel;
};

UCLASS()
class COOKYOURWAY_API UVillageManagerSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	UVillageManagerSystem();

	float TempDelayTime;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Table")
	class UDataTable* CompetitorReviewTable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Table")
	class UDataTable* StoreTable;

	TArray<FCompetitorReviewData*> CompetitorReviewTableRows;
	TArray<FName> CompetitorReviewTableRowNames;

	TArray<FStoreData*> StoreTableRows;

	TArray<FCompetitorReviewData> GetCompetitorReviewDataOnTable(FString DataType);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Day = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PlayerBistroAreaID = 16;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32> CompetitorAreaID = {5, 7, 11, 14, 18, 21};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32> StoreAreaID = { 2, 10, 23 };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FStoreData> LoadedStoreData;

	// 전체 자산
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 TotalAsset = 1000;

	// 플레이어 가게 누적 손님 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PlayerBistroTotalCust = 0;

	// 경쟁사 누적 손님 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<int32, int32> CompetitorTotalCust = {
		{5, 0},
		{7, 0},
		{11, 0},
		{14, 0},
		{18, 0},
		{21, 0}
	};

	// 플레이어 가게 누적 평점 합
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PlayerBistroTotalRateSum = 0;

	// 경쟁사 누적 평점 합
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<int32, int32> CompetitorTotalRateSum = {
		{5, 0},
		{7, 0},
		{11, 0},
		{14, 0},
		{18, 0},
		{21, 0}
	};

	bool DelayWithDeltaTime(float DelayTime, float DeltaSeconds);
};
