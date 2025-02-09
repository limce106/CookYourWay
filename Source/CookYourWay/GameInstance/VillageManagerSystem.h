// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CustomerDataManagerSystem.h"
#include "VillageManagerSystem.generated.h"

USTRUCT(BlueprintType)
struct FCompetitorReviewData : public FTableRowBase {
	GENERATED_BODY()

public:
	FCompetitorReviewData() : CmptState("-1"), RatingMin(-1), RatingMax(-1), RatingProb(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString CmptState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 RatingMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 RatingMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float RatingProb;
};

USTRUCT(BlueprintType)
struct FStoreTable : public FTableRowBase {
	GENERATED_BODY()

public:
	FStoreTable() : StoreName("-1"), StoreScope(-1), StorePeriod(-1), StoreCust1("-1"), StoreCust2("-1"), StoreCust3("-1"),
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

USTRUCT(BlueprintType)
struct FNewsData : public FTableRowBase {
	GENERATED_BODY()

public:
	FNewsData() : NewsCode("-1"), NewsProb(0), NewsString("-1") {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString NewsCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float NewsProb;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString NewsString;
};

USTRUCT(BlueprintType)
struct FCompetitorData
{
	GENERATED_BODY() 
	FCompetitorData() : AreaID(-1), ComptName("-1"), TotalCust(0), RatingAvg(0), OpenPromoDay(3), IsComptFestival(false) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AreaID;	// 부지 번호
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ComptName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalCust;	// 누적 손님 수 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RatingAvg;	// 평균평점
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 OpenPromoDay;	// 오픈 프로모션 남은 기한
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsComptFestival;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCompetitorRatingData> RatingDataArr;

	FCompetitorData(int32 AreaID, float RatingAvg) {
		this->AreaID = AreaID;
		ComptName = "-1";
		TotalCust = 3;
		this->RatingAvg = RatingAvg;
		OpenPromoDay = 3;
		IsComptFestival = false;
	}

	bool operator==(const FCompetitorData& Other) const {
		return AreaID == Other.AreaID;
	}
};

USTRUCT(BlueprintType)
struct FStoreData
{
	GENERATED_BODY()
	FStoreData() : AreaID(-1), StoreTableData(FStoreTable()) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AreaID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FStoreTable StoreTableData;

	FStoreData(int32 AreaID, FStoreTable* StoreTableData) {
		this->AreaID = AreaID;
		this->StoreTableData = *StoreTableData;
	}

	bool operator==(const FStoreData& Other) const {
		return AreaID == Other.AreaID;
	}
};

UCLASS()
class COOKYOURWAY_API UVillageManagerSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	UVillageManagerSystem();

	TArray<FString> RandomComptName = { 
		TEXT("예은이네 뻔뻔샌드"),
		TEXT("샌디네 샌드위치"),
		TEXT("서브웨이"),
		TEXT("홍루이젠")
	};

	float TempDelayTime;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Table")
	class UDataTable* CompetitorReviewTable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Table")
	class UDataTable* StoreTable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Table")
	class UDataTable* NewsTable;

	TArray<FCompetitorReviewData*> CompetitorReviewTableRows;
	TArray<FStoreTable*> StoreTableRows;
	TArray<FNewsData*> NewsTableRows;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PlayerBistroAreaID = 16;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString PlayerBistroName = TEXT("매장 이름");

	// 플레이어 가게 누적 손님 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PlayerBistroTotalCust = 3;

	// 플레이어 가게 평균 평점
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PlayerBistroRating = 2.5;

	// 현재 존재하는 경쟁사 데이터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FCompetitorData> CompetitorDataArr;

	// 현재 존재하는 상점 데이터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FStoreData> StoreDataArr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Day = 0;

	// 전체 자산
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 TotalAsset = 1000;

	// 전체 자산
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 DestroyedCompetitor = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString NewsKeyWord;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString NewsEffectCode;

	UFUNCTION(BlueprintCallable)
	void Init();
	bool DelayWithDeltaTime(float DelayTime, float DeltaSeconds);

	bool IsMonday();
	void ElapseCompetitorOpenPromoDay();

	UFUNCTION(BlueprintCallable)
	int32 FindCompetitorDataArrIdx(int32 AreaID);

	void UpdatePlayerBistroRating(float Rating);
	void InitCompetitorRatingDataArr();
};
