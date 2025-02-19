// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <Actor/CuttingBoard.h>
#include <Actor/Table.h>
#include "Actor/PartTimer.h"
#include "PartTimerAIController.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API APartTimerAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BTAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBlackboardData* BBAsset;

public:
	APartTimerAIController();
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACuttingBoard> BP_CuttingBoard;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATable> BP_Table;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACuttingBoard* HasUnCookedIngrCuttingBoard;
	UFUNCTION(BlueprintCallable)
	bool CheckIfUncookedIngrOnCuttingBoard();
	UFUNCTION(BlueprintCallable)
	void ChopIngrOnCuttingBoard();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ATable* HasCompleteSandwichTable;
	UFUNCTION(BlueprintCallable)
	bool CheckIfCompleteSandwichOnTable();
};
