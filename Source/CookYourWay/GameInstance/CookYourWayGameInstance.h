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

public:
	UCustomerDataManagerSystem* CustomerDataManagerSystem;
	class UVillageManagerSystem* VillageManagerSystem;
	class UIngredientManagerSystem* IngredientManagerSystem;
	
	virtual void Init() override;
	void InitSubSystems();
};
