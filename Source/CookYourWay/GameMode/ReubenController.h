// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Actor/Reuben.h"
#include "ReubenController.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API AReubenController : public APlayerController
{
	GENERATED_BODY()

	AReuben* Reuben;
	AReubenController();

protected:
	virtual void SetupInputComponent() override;

public:

	void Interaction();

	// void PickUp();
	
};
