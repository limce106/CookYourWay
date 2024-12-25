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
	FCompetitorReviewData() : RatingMin(0), RatingMax(0), RatingProb(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 RatingMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 RatingMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float RatingProb;
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

	TArray<FCompetitorReviewData*> CompetitorReviewTableRows;
	TArray<FName> CompetitorReviewTableRowNames;

	TArray<FCompetitorReviewData> GetCompetitorReviewDataOnTable(FString DataType);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Day = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PlayerBistroAreaID = 16;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32> CompetitorAreaID = {5, 7, 11, 14, 18, 21};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32> StoreAreaID = { 2, 10, 23 };

	bool DelayWithDeltaTime(float DelayTime, float DeltaSeconds);
};
