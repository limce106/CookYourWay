// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustomerDataManagerSystem.h"
#include "IngredientManagerSystem.h"
#include "VillageManagerSystem.h"
#include "CookYourWayGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API UCookYourWayGameInstance : public UGameInstance
{
	GENERATED_BODY()

	class UVillageManagerSystem* VillageManagerSystem;
	class UCustomerDataManagerSystem* CustomerDataManagerSystem;
	class UIngredientManagerSystem* IngredientManagerSystem;

public:
	
	virtual void Init() override;
	void InitSubSystems();

	UVillageManagerSystem* GetVillageManagerSystem() const;
	UCustomerDataManagerSystem* GetCustomerDataManagerSystem() const;
	UIngredientManagerSystem* GetIngredientManagerSystem() const;

	static UVillageManagerSystem* GetVillageManagerSystemStatic(UObject* WorldContext);
	static UCustomerDataManagerSystem* GetCustomerDataManagerSystemStatic(UObject* WorldContext);
	static UIngredientManagerSystem* GetIngredientManagerSystemStatic(UObject* WorldContext);
};
