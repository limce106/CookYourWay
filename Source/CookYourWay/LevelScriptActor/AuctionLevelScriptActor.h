// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GameInstance/IngredientManagerSystem.h"
#include "AuctionLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API AAuctionLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32>AuctionIngrTableIdxArr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<int32>SaleIngrTableIdxArr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsContainSClass = false;

	UFUNCTION(BlueprintCallable)
	void SetAuctionItems();
};
