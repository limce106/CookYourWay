// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/IngredientManagerSystem.h"

UIngredientManagerSystem::UIngredientManagerSystem()
{
	FString IngredientTablePath = TEXT("/Game/Assets/Table/Ingredients.Ingredients");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_INGREDIENTTABLE(*IngredientTablePath);
	IngredientTable = DT_INGREDIENTTABLE.Object;

	IngredientTable->GetAllRows<FIngrData>("Get All Rows Of IngrData", IngredientTableRows);
	IngredientTableRowNames = IngredientTable->GetRowNames();

	int index = 0;
	// 재료 타입별 배열 추가
	for (auto Row : IngredientTableRows) {
		IngredientRows.Add(Row);
		IngrNameIndexMap.Add(Row->IngrName, index);

		if (Row->IngrType == "Bread") {
			BreadRows.Add(Row);
		}
		else if (Row->IngrType == "Filling") {
			FillingRows.Add(Row);
		}
		else if (Row->IngrType == "Meat") {
			MeatRows.Add(Row);
		}
		else {
			SauceRows.Add(Row);
		}

		if (Row->IngrClass == "S") {
			SClassIngrRows.Add(index);
		}
		else if (Row->IngrClass == "A") {
			AClassIngrRows.Add(index);
		}
		else if (Row->IngrClass == "B") {
			BClassIngrRows.Add(index);
		}
		else {
			CClassIngrRows.Add(index);
		}

		index++;
	}

	HavingIngrNum.Init(0, IngredientRows.Num());
}

UStaticMesh* UIngredientManagerSystem::GetIngrModel(FString Ingr, bool IsSliced)
{
	FString ModelPath;
	if (IsSliced) {
		ModelPath = FString("/Game/Assets/Art_3D/Modelling/Sandwich/").Append(Ingr).Append("_Slice").Append(".").Append(Ingr).Append("_Slice");
	}
	else {
		ModelPath = FString("/Game/Assets/Art_3D/Modelling/Sandwich/OriginalIngredient/").Append(Ingr).Append(".").Append(Ingr);
	}

	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(NULL, *ModelPath, NULL, LOAD_None, NULL);

	if (!StaticMesh) {
		UE_LOG(LogTemp, Error, TEXT("Can't Find Ingr Model!"));
	}

	return StaticMesh;
}

int32 UIngredientManagerSystem::GetIngrSellingPriceByClass(FString Class)
{
	if (Class == "S")
		return SClassSellingPrice;
	else if (Class == "A")
		return AClassSellingPrice;
	else if (Class == "B")
		return BClassSellingPrice;
	else
		return CClassSellingPrice;
}

int32 UIngredientManagerSystem::GetIngrAuctionPriceByClass(FString Class)
{
	if (Class == "S")
		return SClassAuctionPrice;
	else if (Class == "A")
		return AClassAuctionPrice;
	else if (Class == "B")
		return BClassAuctionPrice;
	else
		return CClassAuctionPrice;
}

int32 UIngredientManagerSystem::GetSellingPriceByIndex(int32 Index)
{
	int32 Price = GetIngrSellingPriceByClass(IngredientRows[Index]->IngrClass);
	return Price;
}

int32 UIngredientManagerSystem::GetAuctionPriceByIndex(int32 Index)
{
	int32 Price = GetIngrAuctionPriceByClass(IngredientRows[Index]->IngrClass);
	return Price;
}
