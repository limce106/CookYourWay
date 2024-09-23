// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ReubenController.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API AReubenController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void SetupInputComponent() override;

public:
	AReubenController();

	void Interaction();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Interaction();
};
