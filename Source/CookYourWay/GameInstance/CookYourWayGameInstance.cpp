// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/CookYourWayGameInstance.h"
#include <Kismet/GameplayStatics.h>

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

UVillageManagerSystem* UCookYourWayGameInstance::GetVillageManagerSystemStatic(UObject* WorldContext)
{
	if (!WorldContext) return nullptr;

	UCookYourWayGameInstance* CookYourWayGameInstance = Cast<UCookYourWayGameInstance>(UGameplayStatics::GetGameInstance(WorldContext));
	return CookYourWayGameInstance ? CookYourWayGameInstance->GetVillageManagerSystem() : nullptr;
}

UCustomerDataManagerSystem* UCookYourWayGameInstance::GetCustomerDataManagerSystemStatic(UObject* WorldContext)
{
	if (!WorldContext) return nullptr;

	UCookYourWayGameInstance* CookYourWayGameInstance = Cast<UCookYourWayGameInstance>(UGameplayStatics::GetGameInstance(WorldContext));
	return CookYourWayGameInstance ? CookYourWayGameInstance->GetCustomerDataManagerSystem() : nullptr;
}

UIngredientManagerSystem* UCookYourWayGameInstance::GetIngredientManagerSystemStatic(UObject* WorldContext)
{
	if (!WorldContext) return nullptr;

	UCookYourWayGameInstance* CookYourWayGameInstance = Cast<UCookYourWayGameInstance>(UGameplayStatics::GetGameInstance(WorldContext));
	return CookYourWayGameInstance ? CookYourWayGameInstance->GetIngredientManagerSystem() : nullptr;
}

UVillageManagerSystem* UCookYourWayGameInstance::GetVillageManagerSystem() const
{
	return VillageManagerSystem;
}

UCustomerDataManagerSystem* UCookYourWayGameInstance::GetCustomerDataManagerSystem() const
{
	return CustomerDataManagerSystem;
}

UIngredientManagerSystem* UCookYourWayGameInstance::GetIngredientManagerSystem() const
{
	return IngredientManagerSystem;
}
