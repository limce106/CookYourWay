// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Actor/Reuben.h"
#include "Actor/CookingUtensil.h"
#include "Actor/Ingredient.h"
#include "Actor/Plate.h"
#include <Actor/Table.h>
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
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATable> BP_Table;

	void Interaction();
	void Chop();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_Chop();

	// void PickUp();
	void EmptyOnSocketInteraction(AActor* InteractActor);
	void PlateOnSocketInteraction(AActor* InteractActor);
	void CookingUtensilOnSocketInteraction(AActor* InteractActor);
	void IngrOnSocketInteraction(AActor* InteractActor);
};
