// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameInstance/VillageManagerSystem.h"
#include "GameInstance/CustomerDataManagerSystem.h"
#include "GameInstance/IngredientManagerSystem.h"
#include "CookYourWayGameMode.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API ACookYourWayGameMode : public AGameMode
{
	GENERATED_BODY()

	class UVillageManagerSystem* VillageManagerSystem;
	class UCustomerDataManagerSystem* CustomerDataManagerSystem;
	class UIngredientManagerSystem* IngredientManagerSystem;
	
protected:
	void InitGame(const FString& MapName, const FString& Option, FString& ErrorMessage) override;
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	TArray<int32> BoughtIngrIdx;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	// ������ ����� <���̺��ε���, ����>
	TMap<int32, int32> BoughtIngrIdxBidPriceMap;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	// ������ ����� <���̺��ε���, ������ ����>
	TMap<int32, int32> BoughtIngrIdxNumMap;
};
