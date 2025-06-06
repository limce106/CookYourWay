// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Subsystems/Subsystem.h"
#include "IngredientManagerSystem.generated.h"

USTRUCT(BlueprintType)
struct FIngrData : public FTableRowBase {
	GENERATED_BODY()

public:
	FIngrData() : IngrName("-1"), IngrClass("-1"), IngrType("-1"), IngrGroupCode(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString IngrName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString IngrClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString IngrType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 IngrGroupCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D* IngrIcon;
};

USTRUCT(BlueprintType)
struct FIngrClassData : public FTableRowBase {
	GENERATED_BODY()

public:
	FIngrClassData() : IngrTasteProb(-1), IngrPrice(-1), IngrBidMin(-1), IngrBidMax(-1), IngrClearSale(-1) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float IngrTasteProb;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 IngrPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 IngrBidMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 IngrBidMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 IngrClearSale;
};

UCLASS()
class COOKYOURWAY_API UIngredientManagerSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UIngredientManagerSystem();
	
public:
	// 재료명, 인덱스 맵
	TMap<FString, int32> IngrNameIndexMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Table")
	class UDataTable* IngredientTable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Table")
	class UDataTable* IngrClassTable;


	TArray<FIngrData*> IngredientTableRows;
	TArray<FName> IngredientTableRowNames;

	TArray<FIngrClassData*> IngrClassTableRows;
	TArray<FName> IngrClassTableRowNames;

	TArray<FIngrData> IngredientRows;
	TArray<FIngrData> BreadRows;
	TArray<FIngrData> FillingRows;
	TArray<FIngrData> MeatRows;
	TArray<FIngrData> SauceRows;
	TArray<FIngrData> DessertRows;

	UPROPERTY()
	TArray<int32> BreadIdxs;
	UPROPERTY()
	TArray<int32> FillingIdxs;
	UPROPERTY()
	TArray<int32> MeatIdxs;
	UPROPERTY()
	TArray<int32> SauceIdxs;
	UPROPERTY()
	TArray<int32> DessertIdxs;

	UPROPERTY()
	TArray<int32> SClassIngrRows;
	UPROPERTY()
	TArray<int32> AClassIngrRows;
	UPROPERTY()
	TArray<int32> BClassIngrRows;
	UPROPERTY()
	TArray<int32> CClassIngrRows;

	// 가지고 있는 재료 개수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32> HavingIngrNum;

	UFUNCTION()
	UStaticMesh* GetIngrModel(FString Ingr, bool IsSliced);

	UFUNCTION(BlueprintCallable)
	int32 GetIngrSellingPrice(FString Class);
	UFUNCTION(BlueprintCallable)
	int32 GetIngrBidMin(FString Class);
	UFUNCTION(BlueprintCallable)
	int32 GetIngrBidMax(FString Class);
	UFUNCTION(BlueprintCallable)
	int32 GetIngrSalePrice(FString Class);

	UFUNCTION()
	int32 GetSellingPriceByIndex(int32 Index);
	UFUNCTION()
	int32 GetIngrBinMinByIndex(int32 Index);

	// 랜덤 취향 배열 반환
	UFUNCTION(BlueprintCallable)
	TArray<int32> GenerateRandomTaste();
};
