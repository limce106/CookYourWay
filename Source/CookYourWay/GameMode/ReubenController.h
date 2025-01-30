// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Actor/Reuben.h"
#include <Actor/Table.h>
#include <Actor/Sandwich.h>
#include <Actor/Ingredient.h>
#include <Actor/FryPan.h>
#include <Actor/CuttingBoard.h>
#include <Actor/DiningTable.h>
#include <Actor/Oven.h>
#include "ReubenController.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCookInteractionDele);

UCLASS()
class COOKYOURWAY_API AReubenController : public APlayerController
{
	GENERATED_BODY()

	AReuben* Reuben;
	AReubenController();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATable> BP_Table;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASandwich> BP_Sandwich;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AIngredient> BP_Ingredient;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFryPan> BP_FryPan;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACuttingBoard> BP_CuttingBoard;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AOven> BP_Oven;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADiningTable> BP_DiningTable;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Fridge;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Plates;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_TrashBin;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> BP_FridgeWidget;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	void Interaction();
	void CookInteraction();

	void FridgeInteraction();
	void PlatesInteraction();
	void TrashBinInteraction();
};
