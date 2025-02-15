// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IngredientManagerSystem.h"
#include "CustomerDataManagerSystem.generated.h"

USTRUCT(BlueprintType)
struct FCustomerData : public FTableRowBase {
	GENERATED_BODY()

public:
	FCustomerData() : CustName("-1") {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString CustName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D* CustIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UStaticMesh* CustModel;
};

USTRUCT(BlueprintType)
struct FCustomerReviewData : public FTableRowBase {
	GENERATED_BODY()

public:
	FCustomerReviewData() : CustCode("-1"), ReviewRating(-1), ReviewString("-1") {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString CustCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ReviewRating;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString ReviewString;
};

USTRUCT(BlueprintType)
struct FCustomerCommentData : public FTableRowBase {
	GENERATED_BODY()

public:
	FCustomerCommentData() : CustCode("-1"), CustCommentType(-1), CustCommentString("-1") {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString CustCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 CustCommentType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString CustCommentString;
};

USTRUCT(BlueprintType)
struct FCompetitorRatingData
{
	GENERATED_BODY()
	FCompetitorRatingData() : CustName("-1"), WeekDay("-1"), Rating(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CustName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeekDay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rating;

	FCompetitorRatingData(FString CustName, FString WeekDay, float Rating) {
		this->CustName = CustName;
		this->WeekDay = WeekDay;
		this->Rating = Rating;
	}
};

USTRUCT(BlueprintType)
struct FPlayerBistroRatingData
{
	GENERATED_BODY()
	FPlayerBistroRatingData() : CustName("-1"), WeekDay("-1"), Rating(0), Price(0), Review(" "), GivenIngr(), IsEatDessert(false), IsFastCook(false), IsSlowCook(false), IsTalk(false) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CustName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WeekDay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rating;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Review;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> GivenIngr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsEatDessert;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFastCook;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSlowCook;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsTalk;
};

USTRUCT(BlueprintType)
struct FCustomerBistroKey
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CustomerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BistroAreaID;

	bool operator==(const FCustomerBistroKey& Other) const {
		return CustomerName == Other.CustomerName && BistroAreaID == Other.BistroAreaID;
	}
};

FORCEINLINE uint32 GetTypeHash(const FCustomerBistroKey& Key)
{
	return HashCombine(GetTypeHash(Key.CustomerName), GetTypeHash(Key.BistroAreaID));
}

UCLASS()
class COOKYOURWAY_API UCustomerDataManagerSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	UCustomerDataManagerSystem();

	class UVillageManagerSystem* VillageManagerSystem;
	class UIngredientManagerSystem* IngredientManagerSystem;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:

	UFUNCTION(BlueprintCallable)
	void Init();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Table")
	class UDataTable* CustomerTable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Table")
	class UDataTable* CustomerReviewTable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Table")
	class UDataTable* CustomerCommentTable;

	TArray<FCustomerData*> CustomerTableRows;
	TArray<FCustomerReviewData*> CustomerReviewTableRows;
	TArray<FCustomerCommentData*> CustomerCommentTableRows;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> CustomerNames;

	// <손님 이름, 취향>
	TMap<FString, TArray<int32>> CustNameToTasteMap;
	// <손님 이름/가게, 단골여부>
	TMap<FCustomerBistroKey, bool> IsRegularCustMap;
	// <손님 이름/가게, 충성도>
	TMap<FCustomerBistroKey, int32> LoyaltyMap;
	// <손님 이름/가게, 최대만족도>
	TMap<FCustomerBistroKey, int32> MaxSatisfactionMap;
	// <손님 이름/경쟁사, 최대만족도를 얻었던 조합> 
	TMap<FCustomerBistroKey, TArray<int32>> CmptBestRatedCombos;

	// 플레이어 가게 리뷰 데이터
	UPROPERTY(VisibleAnywhere)
	TArray<FPlayerBistroRatingData> PlayerBistroRatingDataArr;

	// 최대 평점 
	const float MaxRate = 5.0;

	UFUNCTION(BlueprintCallable)
	FCustomerBistroKey GetCustomerBistroKey(FString CustomerName, int32 BistroAreaID);
	
	// 랜덤 손님 이름 가져오기
	FString GetRandomCustName();

	// 랜덤으로 모든 손님 취향 설정
	UFUNCTION(BlueprintCallable)
	void SetRandomCustTastes();
	// 식재료 페스티벝 진행 중 모든 손님 취향 설정
	UFUNCTION(BlueprintCallable)
	void SetIngrSeasonCustTastes();
	// 인기있는 재료로 모든 손님 취향 설정
	UFUNCTION(BlueprintCallable)
	void SetPopularIngrCustTastes();
	// 모든 손님 유형의 취향 설정
	UFUNCTION(BlueprintCallable)
	void SetCustTastes();
	// 특정 손님의 취향 가져오기
	TArray<int32> GetCustTaste(FString CustName);

	bool UpdateMaxSatisfaction(FString CustName, int32 BistroAreaID, int32 Satisfaction);
	void UpdateCmptBestRatedCombos(FString CustName, int32 BistroAreaID, TArray<int32> Ingr);

	// 단골 손님 여부 반환
	bool IsRegularCust(FString CustomerName, int32 BistroAreaID);
	// 단골 손님 추가
	void AddRegularCust(FString CustomerName, int32 BistroAreaID);
	// 충성도 감소
	void DecreaseLoyalty(FString CustomerName, int32 BistroAreaID, float Decreasement);
	// 경쟁사의 단골 손님 충성도 감소
	void DecreaseCompetitorLoyalty(int32 CompetitorAreaID);
	// 경쟁사의 단골 손님 랜덤으로 추가
	void AddCompetitorRegularCust();
	// 단골 손님 보유 여부
	bool HasRegularCust(int32 BistroAreaID);

	// 손님의 취향 힌트 대사 재정의
	UFUNCTION(BlueprintCallable)
	void RedefineCustomerComment();
	UFUNCTION(BlueprintCallable)
	FString RedefineTasteHintComment(FString CustName, FString Comment);
	UFUNCTION(BlueprintCallable)
	FString GetCustReviewDialogue(FString CustName, int32 TasteScore);

	UFUNCTION(BlueprintCallable)
	TArray<int32> GetCustNameToTasteMapValue(FString CustName);
	UFUNCTION(BlueprintCallable)
	bool GetIsRegularCustMapValue(FCustomerBistroKey Key);
	UFUNCTION(BlueprintCallable)
	int32 GetLoyaltyMapValue(FCustomerBistroKey Key);
	UFUNCTION(BlueprintCallable)
	int32 GetMaxSatisfactionMapValue(FCustomerBistroKey Key);
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetCmptBestRatedCombos(FCustomerBistroKey Key);

	UFUNCTION(BlueprintCallable)
	TArray<FPlayerBistroRatingData> GetGreaterSortedPlayerBistroRating();
};
