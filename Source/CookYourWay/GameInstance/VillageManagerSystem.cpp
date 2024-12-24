// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/VillageManagerSystem.h"
#include <Kismet/KismetMathLibrary.h>
#include "CustomerDataManagerSystem.h"
#include <Kismet/GameplayStatics.h>

UVillageManagerSystem::UVillageManagerSystem()
{
	FString CompetitorReviewTablePath = TEXT("/Game/Assets/Table/CompetitorReview.CompetitorReview");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_COMPETITORTABLE(*CompetitorReviewTablePath);
	CompetitorReviewTable = DT_COMPETITORTABLE.Object;

	CompetitorReviewTable->GetAllRows<FCompetitorReviewData>("Get All Rows Of IngrData", CompetitorReviewTableRows);
	CompetitorReviewTableRowNames = CompetitorReviewTable->GetRowNames();
}

void UVillageManagerSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

TArray<FCompetitorReviewData> UVillageManagerSystem::GetCompetitorReviewDataOnTable(FString DataType)
{
	TArray<FCompetitorReviewData> CompetitorReviewData;

	for (int i = 0; i < CompetitorReviewTableRowNames.Num(); i++) {
		if (CompetitorReviewTableRowNames[i].ToString().Contains(DataType)) {
			CompetitorReviewData.Add(*CompetitorReviewTableRows[i]);
		}
	}

	if (CompetitorReviewData.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("Load Fail %s CompetitorReviewData"), *DataType);
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
