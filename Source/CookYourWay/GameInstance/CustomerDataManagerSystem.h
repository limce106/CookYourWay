// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VillageManagerSystem.h"
#include "IngredientManagerSystem.h"
#include "CustomerDataManagerSystem.generated.h"

USTRUCT(BlueprintType)
struct FCustomerData : public FTableRowBase {
	GENERATED_BODY()

public:
	FCustomerData() : CustName("-1"), CustTasteCount(-1), CustUnqTaste(-1) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString CustName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 CustTasteCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 CustUnqTaste;
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

	// <�մ� �̸�, ����>
	TMap<FString, TArray<int32>> CustNameToTasteMap;
	// <�մ� �̸�/����, �ܰ񿩺�>
	TMap<FCustomerBistroKey, bool> IsRegularCustMap;
	// <�մ� �̸�/����, �漺��>
	TMap<FCustomerBistroKey, int32> LoyaltyMap;
	// <�մ� �̸�/����, �ִ븸����>
	TMap<FCustomerBistroKey, int32> MaxSatisfactionMap;

	// �ִ� ���� 
	const float MaxRate = 5.0;

	UFUNCTION(BlueprintCallable)
	FCustomerBistroKey GetCustomerBistroKey(FString CustomerName, int32 BistroAreaID);
	
	// ���� �մ� �̸� ��������
	FString GetRandomCustName();

	// ���� ���� �迭 ��ȯ
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetRandomTaste();
	// �������� ��� �մ� ���� ����
	UFUNCTION(BlueprintCallable)
	void SetRandomCustTastes();
	// ����� �佺Ƽ�� ���� �� ��� �մ� ���� ����
	UFUNCTION(BlueprintCallable)
	void SetIngrSeasonCustTastes();
	// �α��ִ� ���� ��� �մ� ���� ����
	UFUNCTION(BlueprintCallable)
	void SetPopularIngrCustTastes();
	// ��� �մ� ������ ���� ����
	UFUNCTION(BlueprintCallable)
	void SetCustTastes();
	// Ư�� �մ��� ���� ��������
	TArray<int32> GetCustTaste(FString CustName);

	void UpdateMaxSatisfaction(FString CustName, int32 BistroAreaID, int32 Satisfaction);

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

	UFUNCTION(BlueprintCallable)
	FString GetCustReviewDialogue(FString CustName, int32 TasteScore);
	UFUNCTION(BlueprintCallable)
	bool GetIsRegularCustMapValue(FCustomerBistroKey Key);
	UFUNCTION(BlueprintCallable)
	int32 GetLoyaltyMapValue(FCustomerBistroKey Key);
	UFUNCTION(BlueprintCallable)
	int32 GetMaxSatisfactionMapValue(FCustomerBistroKey Key);

	/*UFUNCTION(BlueprintCallable)
	TArray<FCompetitorRatingData> GetSortedArrByGreaterRating(TArray<FCompetitorRatingData> RatingArr);*/
};
