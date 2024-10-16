// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/IngredientManagerSystem.h"

UIngredientManagerSystem::UIngredientManagerSystem()
{
	FString IngredientTablePath = TEXT("/Game/Assets/Table/Ingredients.Ingredients");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_INGREDIENTTABLE(*IngredientTablePath);
	IngredientTable = DT_INGREDIENTTABLE.Object;

	IngredientTable->GetAllRows<FIngrData>("Get All Rows Of IngrData", IngredientTableRows);
	IngredientTableRowNames = IngredientTable->GetRowNames();

	// ��� Ÿ�Ժ� �迭 �߰�
	for (auto Row : IngredientTableRows) {
		IngredientRows.Add(Row);

		if (Row->IngrType == "Filling") {
			FillingRows.Add(Row);
		}
		else if (Row->IngrType == "Meat") {
			MeatRows.Add(Row);
		}
		else {
			SauceRows.Add(Row);
		}
	}
}

UStaticMesh* UIngredientManagerSystem::GetIngrModel(FString Ingr)
{
	FString ModelPath = FString("/Game/Assets/Art_3D/Modelling/Sandwich/Ingredient/").Append(Ingr).Append(".").Append(Ingr);
	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(NULL, *ModelPath, NULL, LOAD_None, NULL);

	return StaticMesh;
}
