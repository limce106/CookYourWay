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

		if (Row->IngrType == "Filling") {
			FillingRows.Add(Row);
		}
		else if (Row->IngrType == "Meat") {
			MeatRows.Add(Row);
		}
		else {
			SauceRows.Add(Row);
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
