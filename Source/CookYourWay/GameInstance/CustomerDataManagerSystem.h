// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VillageManagerSystem.h"
#include "IngredientManagerSystem.h"
#include "CustomerDataManagerSystem.generated.h"

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

	class UVillageManagerSystem* VillageManagerSystem;
	class UIngredientManagerSystem* IngredientManagerSystem;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:

	void Init();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> CustomerNames = {
		"Amy",
		"Michelle"
	};

	// <손님 이름, 취향>
	TMap<FString, TArray<int32>> CustNameToTasteMap;
	// <손님 이름/가게, 단골여부>
	TMap<FCustomerBistroKey, bool> IsRegularCustMap;
	// <손님 이름/가게, 충성도>
	TMap<FCustomerBistroKey, float> LoyaltyMap;
	// <손님 이름/가게, 평점평균>
	TMap<FCustomerBistroKey, float> AvgRateMap;
	// <손님 이름/가게, 해당 유형 손님이 방문한 횟수>
	TMap<FCustomerBistroKey, int32> VisitedNumMap;

	// 최대 평점 
	const float MaxRate = 5.0;

	FCustomerBistroKey GetCustomerBistroKey(FString CustomerName, int32 BistroAreaID);
	
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

	float GetAvgRate(FString CustomerName, int32 BistroAreaID);
	float GetLoyalty(FString CustomerName, int32 BistroAreaID);

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

	// 손님 유형별 평점 평균 갱신
	// void UpdateAvgRateByCustName(FString CustomerName, int32 BistroAreaID, int32 Satisfaction);
	//void UpdateTodayAvgRate();
	// void AddTodaySatisfactionMap(FString CustomerName, int32 BistroAreaID, int32 Satisfaction);
};
