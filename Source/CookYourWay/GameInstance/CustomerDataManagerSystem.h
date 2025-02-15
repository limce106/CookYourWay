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

	// <�մ� �̸�, ����>
	TMap<FString, TArray<int32>> CustNameToTasteMap;
	// <�մ� �̸�/����, �ܰ񿩺�>
	TMap<FCustomerBistroKey, bool> IsRegularCustMap;
	// <�մ� �̸�/����, �漺��>
	TMap<FCustomerBistroKey, int32> LoyaltyMap;
	// <�մ� �̸�/����, �ִ븸����>
	TMap<FCustomerBistroKey, int32> MaxSatisfactionMap;
	// <�մ� �̸�/�����, �ִ븸������ ����� ����> 
	TMap<FCustomerBistroKey, TArray<int32>> CmptBestRatedCombos;

	// �÷��̾� ���� ���� ������
	UPROPERTY(VisibleAnywhere)
	TArray<FPlayerBistroRatingData> PlayerBistroRatingDataArr;

	// �ִ� ���� 
	const float MaxRate = 5.0;

	UFUNCTION(BlueprintCallable)
	FCustomerBistroKey GetCustomerBistroKey(FString CustomerName, int32 BistroAreaID);
	
	// ���� �մ� �̸� ��������
	FString GetRandomCustName();

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

	bool UpdateMaxSatisfaction(FString CustName, int32 BistroAreaID, int32 Satisfaction);
	void UpdateCmptBestRatedCombos(FString CustName, int32 BistroAreaID, TArray<int32> Ingr);

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

	// �մ��� ���� ��Ʈ ��� ������
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
