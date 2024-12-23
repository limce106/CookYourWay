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
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UTexture2D* IngrIcon;*/
};

UCLASS()
class COOKYOURWAY_API UIngredientManagerSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UIngredientManagerSystem();
	
public:
	// 재료명, 인덱스 맵
	TMap<FString, int32> IngrNameIndexMap;

	// 빵 인덱스, *데이터 테이블 행 순서가 바뀔 것을 고려하여 - 수정 필요*
	const int BreadIndex = 4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Table")
	class UDataTable* IngredientTable;

	TArray<FIngrData*> IngredientTableRows;
	TArray<FName> IngredientTableRowNames;

	TArray<FIngrData*> IngredientRows;
	TArray<FIngrData*> FillingRows;
	TArray<FIngrData*> MeatRows;
	TArray<FIngrData*> SauceRows;

	// 가지고 있는 재료 개수
	TArray<int32> HavingIngrNum;

	UStaticMesh* GetIngrModel(FString Ingr, bool IsSliced);
};
