// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Actor/Reuben.h"
#include "Actor/CookingUtensil.h"
#include "Actor/Ingredient.h"
#include "Actor/Plate.h"
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
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AIngredient> BP_Ingredient;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACookingUtensil> BP_CookingUtensil;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlate> BP_Plate;

	void Interaction();
	void Chop();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Interaction();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Chop();

	void PickUp();
};
