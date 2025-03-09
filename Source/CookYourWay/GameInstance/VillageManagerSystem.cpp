// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/VillageManagerSystem.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>

UVillageManagerSystem::UVillageManagerSystem()
{
	FString CompetitorReviewTablePath = TEXT("/Game/Assets/Table/CompetitorReview.CompetitorReview");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_COMPETITORREVIEWTABLE(*CompetitorReviewTablePath);
	CompetitorReviewTable = DT_COMPETITORREVIEWTABLE.Object;
	CompetitorReviewTable->GetAllRows<FCompetitorReviewData>("Get All Rows Of CompetitorReviewData", CompetitorReviewTableRows);

	FString StoreTablePath = TEXT("/Game/Assets/Table/Store.Store");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_STORETABLE(*StoreTablePath);
	StoreTable = DT_STORETABLE.Object;
	StoreTable->GetAllRows<FStoreTable>("Get All Rows Of StoreTableData", StoreTableRows);

	FString NewsTablePath = TEXT("/Game/Assets/Table/News.News");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_NEWSTABLE(*NewsTablePath);
	NewsTable = DT_NEWSTABLE.Object;
	NewsTable->GetAllRows<FNewsData>("Get All Rows Of NewsData", NewsTableRows);
}

void UVillageManagerSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UVillageManagerSystem::Init()
{
	TArray<FString> CopyRandomComptName = RandomComptName;
	TArray<int32> CompetitorAreaIDArr = { 5, 18 };
	bool IsCompetitorDataEmpty = CompetitorDataArr.IsEmpty();

	if (IsCompetitorDataEmpty) {
		float InitCompetitorRating;

		for (int i = 0; i < CompetitorAreaIDArr.Num(); i++) {
			if (i == 0) {
				InitCompetitorRating = 3.5;
			}
			else {
				InitCompetitorRating = 2.5;
			}

			FCompetitorData CompetitorData(CompetitorAreaIDArr[i], InitCompetitorRating);

			int32 ComptNameIdx = UKismetMathLibrary::RandomIntegerInRange(0, CopyRandomComptName.Num() - 1);
			CompetitorData.ComptName = CopyRandomComptName[ComptNameIdx];
			CopyRandomComptName.RemoveAt(ComptNameIdx);

			CompetitorDataArr.Add(CompetitorData);
		}
	}

	TArray<int32> StoreAreaID = { 2, 10, 23 };
	bool IsStoreDataEmpty = StoreDataArr.IsEmpty();
	if (IsStoreDataEmpty) {
		for (int i = 0; i < StoreAreaID.Num(); i++) {
			int32 RandomStoreIdx = UKismetMathLibrary::RandomIntegerInRange(0, StoreTableRows.Num() - 1);
			FStoreData StoreData(StoreAreaID[i], StoreTableRows[RandomStoreIdx]);
			StoreDataArr.Add(StoreData);
		}
	}
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

bool UVillageManagerSystem::IsMonday()
{
	bool IsMonday = Day != 1 && Day % 7 == 1;
	return IsMonday;
}

bool UVillageManagerSystem::IsSunday()
{
	bool IsSunday =  Day % 7 == 0;
	return IsSunday;
}

void UVillageManagerSystem::ElapseCompetitorOpenPromoDay()
{
	for (int i = 0; i < CompetitorDataArr.Num(); i++) {
		if (CompetitorDataArr[i].OpenPromoDay == 0) {
			continue;
		}

		int32 DecreasedRemainDay = CompetitorDataArr[i].OpenPromoDay - 1;
		CompetitorDataArr[i].OpenPromoDay = DecreasedRemainDay;
	}
}

void UVillageManagerSystem::ElapseStorePeriod()
{
	for (int i = 0; i < StoreDataArr.Num(); i++) {
		if (StoreDataArr[i].StoreTableData.StorePeriod == 0) {
			continue;
		}

		int32 DecreasedRemainDay = StoreDataArr[i].StoreTableData.StorePeriod - 1;
		StoreDataArr[i].StoreTableData.StorePeriod = DecreasedRemainDay;
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

void UVillageManagerSystem::UpdatePlayerBistroRating(float Rating)
{
	float UpdatedRating = ((PlayerBistroRating * (PlayerBistroTotalCust - 1)) + Rating) / (float)PlayerBistroTotalCust;
	PlayerBistroRating = FMath::RoundToFloat(UpdatedRating * 10.0f) / 10.0f;
}

void UVillageManagerSystem::InitCompetitorRatingDataArr()
{
	for (auto CompetitorData : CompetitorDataArr) {
		CompetitorData.RatingDataArr.Empty();
	}
}

FString UVillageManagerSystem::GetRandomComptName()
{
	int32 ComptNameIdx = UKismetMathLibrary::RandomIntegerInRange(0, RandomComptName.Num() - 1);
	return RandomComptName[ComptNameIdx];
}