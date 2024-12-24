// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/CookYourWayGameInstance.h"

void UCookYourWayGameInstance::Init()
{
	Super::Init();

	InitSubSystems();
}

void UCookYourWayGameInstance::InitSubSystems()
{
	VillageManagerSystem = GetSubsystem<UVillageManagerSystem>();
	IngredientManagerSystem = GetSubsystem<UIngredientManagerSystem>();
	CustomerDataManagerSystem = GetSubsystem<UCustomerDataManagerSystem>();
}
