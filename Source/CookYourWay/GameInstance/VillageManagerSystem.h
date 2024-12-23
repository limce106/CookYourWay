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

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Table")
	class UDataTable* CompetitorReviewTable;

	TArray<FCompetitorReviewData*> CompetitorReviewTableRows;
	TArray<FName> CompetitorReviewTableRowNames;

	TArray<FCompetitorReviewData> GetCompetitorReviewDataOnTable(FString DataType);

	TArray<FString> CustomerNames = {
		"Amy",
		"Michelle"
	};

	// ·£´ý ¼Õ´Ô ÀÌ¸§ °¡Á®¿À±â
	FString GetRandomCustName();

	bool DelayWithDeltaTime(float DelayTime, float DeltaSeconds);
};
