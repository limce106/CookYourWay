// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/IngredientManagerSystem.h"
#include <Kismet/KismetMathLibrary.h>

UIngredientManagerSystem::UIngredientManagerSystem()
{
	FString IngredientTablePath = TEXT("/Game/Assets/Table/Ingredients.Ingredients");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_INGREDIENTTABLE(*IngredientTablePath);
	IngredientTable = DT_INGREDIENTTABLE.Object;

	FString IngrClassTablePath = TEXT("/Game/Assets/Table/IngredientsClass.IngredientsClass");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_INGREDIENTCLASSTABLE(*IngrClassTablePath);
	IngrClassTable = DT_INGREDIENTCLASSTABLE.Object;

	IngredientTable->GetAllRows<FIngrData>("Get All Rows Of IngrData", IngredientTableRows);
	IngredientTableRowNames = IngredientTable->GetRowNames();

	IngrClassTable->GetAllRows<FIngrClassData>("Get All Rows Of IngrClassData", IngrClassTableRows);
	IngrClassTableRowNames = IngrClassTable->GetRowNames();

	int index = 0;
	// 재료 타입별 배열 추가
	for (auto Row : IngredientTableRows) {
		IngredientRows.Add(*Row);
		IngrNameIndexMap.Add(Row->IngrName, index);

		if (Row->IngrType == "Bread") {
			BreadRows.Add(*Row);
			BreadIdxs.Add(index);
		}
		else if (Row->IngrType == "Filling") {
			FillingRows.Add(*Row);
			FillingIdxs.Add(index);
		}
		else if (Row->IngrType == "Meat") {
			MeatRows.Add(*Row);
			MeatIdxs.Add(index);
		}
		else if (Row->IngrType == "Sauce") {
			SauceRows.Add(*Row);
			SauceIdxs.Add(index);
		}
		else {
			DessertRows.Add(*Row);
			DessertIdxs.Add(index);
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

	HavingIngrNum.Init(5, IngredientRows.Num());
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

int32 UIngredientManagerSystem::GetIngrSellingPrice(FString Class)
{
	FIngrClassData* IngrClassData = IngrClassTable->FindRow<FIngrClassData>(FName(*Class), TEXT(""));
	return IngrClassData->IngrPrice;
}

int32 UIngredientManagerSystem::GetIngrBidMin(FString Class)
{
	FIngrClassData* IngrClassData = IngrClassTable->FindRow<FIngrClassData>(FName(*Class), TEXT(""));
	return IngrClassData->IngrBidMin;
}

int32 UIngredientManagerSystem::GetIngrBidMax(FString Class)
{
	FIngrClassData* IngrClassData = IngrClassTable->FindRow<FIngrClassData>(FName(*Class), TEXT(""));
	return IngrClassData->IngrBidMax;
}

int32 UIngredientManagerSystem::GetIngrSalePrice(FString Class)
{
	FIngrClassData* IngrClassData = IngrClassTable->FindRow<FIngrClassData>(FName(*Class), TEXT(""));
	return IngrClassData->IngrClearSale;
}

int32 UIngredientManagerSystem::GetSellingPriceByIndex(int32 Index)
{
	int32 Price = GetIngrSellingPrice(IngredientRows[Index].IngrClass);
	return Price;
}

int32 UIngredientManagerSystem::GetIngrBinMinByIndex(int32 Index)
{
	int32 Price = GetIngrBidMin(IngredientRows[Index].IngrClass);
	return Price;
}

TArray<int32> UIngredientManagerSystem::GenerateRandomTaste()
{
	TArray<int32> Taste;

	TArray<int32> CanAddFillings = FillingIdxs;
	// 속재료는 항상 3개를 선택하도록 함
	for (int i = 0; i < 3; i++) {
		int FillingIndex = UKismetMathLibrary::RandomIntegerInRange(0, CanAddFillings.Num() - 1);
		Taste.Add(CanAddFillings[FillingIndex]);
		CanAddFillings.RemoveAt(FillingIndex);
	}

	int MeatIndex = UKismetMathLibrary::RandomIntegerInRange(0, MeatIdxs.Num() - 1);
	Taste.Add(MeatIdxs[MeatIndex]);

	int SauceIndex = UKismetMathLibrary::RandomIntegerInRange(0, SauceIdxs.Num() - 1);
	Taste.Add(SauceIdxs[SauceIndex]);

	// 테스트
	//Taste.Add(13);
	//

	return Taste;
}
