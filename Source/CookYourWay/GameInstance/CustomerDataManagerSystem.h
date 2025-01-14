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

	// <�մ� �̸�, ����>
	TMap<FString, TArray<int32>> CustNameToTasteMap;
	// <�մ� �̸�/����, �ܰ񿩺�>
	TMap<FCustomerBistroKey, bool> IsRegularCustMap;
	// <�մ� �̸�/����, �漺��>
	TMap<FCustomerBistroKey, float> LoyaltyMap;
	// <�մ� �̸�/����, �������>
	TMap<FCustomerBistroKey, float> AvgRateMap;
	// <�մ� �̸�/����, �ش� ���� �մ��� �湮�� Ƚ��>
	TMap<FCustomerBistroKey, int32> VisitedNumMap;

	// �ִ� ���� 
	const float MaxRate = 5.0;

	FCustomerBistroKey GetCustomerBistroKey(FString CustomerName, int32 BistroAreaID);
	
	// ���� �մ� �̸� ��������
	FString GetRandomCustName();

	// ���� ���� �迭 ��ȯ
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetRandomTaste();
	// ��� �մ� ������ ���� ����
	UFUNCTION(BlueprintCallable)
	void SetAllCustTastes();
	// Ư�� �մ��� ���� ��������
	TArray<int32> GetCustTaste(FString CustName);

	float GetAvgRate(FString CustomerName, int32 BistroAreaID);
	float GetLoyalty(FString CustomerName, int32 BistroAreaID);

	// �ܰ� �մ� ���� ��ȯ
	bool IsRegularCust(FString CustomerName, int32 BistroAreaID);
	// �ܰ� �մ� �߰�
	void AddRegularCust(FString CustomerName, int32 BistroAreaID);
	// �漺�� ����
	void DecreaseLoyalty(FString CustomerName, int32 BistroAreaID, float Decreasement);
	// ������� �ܰ� �մ� �漺�� ����
	void DecreaseCompetitorLoyalty(int32 CompetitorAreaID);
	// ������� �ܰ� �մ� �������� �߰�
	void AddCompetitorRegularCust();
	// �ܰ� �մ� ���� ����
	bool HasRegularCust(int32 BistroAreaID);

	// �մ� ������ ���� ��� ����
	// void UpdateAvgRateByCustName(FString CustomerName, int32 BistroAreaID, int32 Satisfaction);
	//void UpdateTodayAvgRate();
	// void AddTodaySatisfactionMap(FString CustomerName, int32 BistroAreaID, int32 Satisfaction);
};
