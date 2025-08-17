// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"


/**
 * 
 */
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	//GENERATED_IINTERFACE_BODY()
};

class IInteractable
{
	//GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void Interact(class AReuben* Reuben);
};