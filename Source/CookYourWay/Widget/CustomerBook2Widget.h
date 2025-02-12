// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomerBook2Widget.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API UCustomerBook2Widget : public UUserWidget
{
	GENERATED_BODY()
	class UVillageManagerSystem* VillageManagerSystem;

	void NativeConstruct();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Init();
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetSpawnedStoreName(FString CustName);

};
