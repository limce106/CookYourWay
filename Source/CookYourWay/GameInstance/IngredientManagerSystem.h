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
	// ����, �ε��� ��
	TMap<FString, int32> IngrNameIndexMap;

	// �� �ε���, *������ ���̺� �� ������ �ٲ� ���� ����Ͽ� - ���� �ʿ�*
	const int32 BreadIndex = 4;

	// �ǸŰ�
	const int32 SClassSellingPrice = 50;
	const int32 AClassSellingPrice = 40;
	const int32 BClassSellingPrice = 30;
	const int32 CClassSellingPrice = 20;

	// ��� ���۰�
	const int32 SClassAuctionPrice = 20;
	const int32 AClassAuctionPrice = 20;
	const int32 BClassAuctionPrice = 15;
	const int32 CClassAuctionPrice = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Table")
	class UDataTable* IngredientTable;

	TArray<FIngrData*> IngredientTableRows;
	TArray<FName> IngredientTableRowNames;

	TArray<FIngrData*> IngredientRows;
	TArray<FIngrData*> BreadRows;
	TArray<FIngrData*> FillingRows;
	TArray<FIngrData*> MeatRows;
	TArray<FIngrData*> SauceRows;

	TArray<FIngrData*> SClassIngrRows;
	TArray<FIngrData*> AClassIngrRows;
	TArray<FIngrData*> BClassIngrRows;
	TArray<FIngrData*> CClassIngrRows;

	// ������ �ִ� ��� ����
	TArray<int32> HavingIngrNum;

	UStaticMesh* GetIngrModel(FString Ingr, bool IsSliced);
};
