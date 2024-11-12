// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/VillageManagerSystem.h"

UVillageManagerSystem::UVillageManagerSystem()
{
	FString CompetitorReviewTablePath = TEXT("/Game/Assets/Table/CompetitorReview.CompetitorReview");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_COMPETITORTABLE(*CompetitorReviewTablePath);
	CompetitorReviewTable = DT_COMPETITORTABLE.Object;

	CompetitorReviewTable->GetAllRows<FCompetitorReviewData>("Get All Rows Of IngrData", CompetitorReviewTableRows);
	CompetitorReviewTableRowNames = CompetitorReviewTable->GetRowNames();
}

TArray<FCompetitorReviewData> UVillageManagerSystem::GetCompetitorReviewDataOnTable(FString DataType)
{
	TArray<FCompetitorReviewData> CompetitorReviewData;

	for (int i = 0; i < CompetitorReviewTableRowNames.Num(); i++) {
		if (CompetitorReviewTableRowNames[i].ToString().Contains(DataType)) {
			CompetitorReviewData.Add(*CompetitorReviewTableRows[i]);
		}
		else
			break;
	}

	if (CompetitorReviewData.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("Load fail"));
		CompetitorReviewData.Add(FCompetitorReviewData::FCompetitorReviewData());
	}
	return CompetitorReviewData;
}

bool UVillageManagerSystem::DelayWithDeltaTime(float DelayTime, float DeltaSeconds)
{
	if (TempDelayTime > DelayTime) {
		TempDelayTime = 0;
		return true;
	}
	else {
		TempDelayTime += DeltaSeconds;
		return false;
	}
}
