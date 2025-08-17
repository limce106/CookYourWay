// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Holdable.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UHoldable : public UInterface
{
	GENERATED_BODY()
};

class IHoldable
{
	GENERATED_BODY()

public:
	// 내려놓기
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnPutDown(AActor* PlaceTarget);

	// 들기
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnPickUp();
};
