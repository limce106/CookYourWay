// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/VillageManagerSystem.h"
#include <Kismet/KismetMathLibrary.h>
#include "CustomerDataManagerSystem.h"
#include <Kismet/GameplayStatics.h>

UVillageManagerSystem::UVillageManagerSystem()
{
	FString CompetitorReviewTablePath = TEXT("/Game/Assets/Table/CompetitorReview.CompetitorReview");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_COMPETITORREVIEWTABLE(*CompetitorReviewTablePath);
	CompetitorReviewTable = DT_COMPETITORREVIEWTABLE.Object;

	CompetitorReviewTable->GetAllRows<FCompetitorReviewData>("Get All Rows Of CompetitorReviewData", CompetitorReviewTableRows);
	CompetitorReviewTableRowNames = CompetitorReviewTable->GetRowNames();

	FString StoreTablePath = TEXT("/Game/Assets/Table/Store.Store");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_STORETABLE(*StoreTablePath);
	StoreTable = DT_STORETABLE.Object;

	StoreTable->GetAllRows<FStoreTable>("Get All Rows Of StoreTableData", StoreTableRows);
}

void UVillageManagerSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void UVillageManagerSystem::Init()
{
	TArray<int32> CompetitorAreaIDArr = { 5, 7, 11, 14, 18, 21 };
	bool IsCompetitorDataEmpty = CompetitorDataArr.IsEmpty();
	if (IsCompetitorDataEmpty) {
		for (int i = 0; i < 6; i++) {
			FCompetitorData CompetitorData(CompetitorAreaIDArr[i]);
			CompetitorDataArr.Add(CompetitorData);
		}
	}

	TArray<int32> StoreAreaID = { 2, 10, 23 };
	bool IsStoreDataEmpty = StoreDataArr.IsEmpty();
	if (IsStoreDataEmpty) {
		for (int i = 0; i < 3; i++) {
			int32 RandomStoreIdx = UKismetMathLibrary::RandomIntegerInRange(0, StoreTableRows.Num() - 1);
			FStoreData StoreData(StoreAreaID[i], StoreTableRows[RandomStoreIdx]);
			StoreDataArr.Add(StoreData);
		}
	}
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

void UVillageManagerSystem::DecreaseCompetitorOpenPromoDay()
{
	for (int i = 0; i < CompetitorDataArr.Num(); i++) {
		if (CompetitorDataArr[i].OpenPromoDay == 0) {
			continue;
		}

		int32 DecreasedRemainDay = CompetitorDataArr[i].OpenPromoDay - 1;
		CompetitorDataArr[i].OpenPromoDay = DecreasedRemainDay;
	}
}

int32 UVillageManagerSystem::FindCompetitorDataArrIdx(int32 AreaID)
{
	int32 Idx = -1;
	for (int i = 0; i < CompetitorDataArr.Num(); i++) {
		if (CompetitorDataArr[i].AreaID == AreaID) {
			Idx = i;
			break;
		}
	}

	if (Idx == -1) {
		UE_LOG(LogTemp, Error, TEXT("Can't Find Competitor Data Arr Index!"));
	}

	return Idx;
}
