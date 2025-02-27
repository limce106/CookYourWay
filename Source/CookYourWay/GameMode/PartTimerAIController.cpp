// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PartTimerAIController.h"
#include "BehaviorTree//BlackboardComponent.h"
#include "BehaviorTree//BehaviorTree.h"
#include "BehaviorTree//BlackboardData.h"
#include <Kismet/GameplayStatics.h>
#include "Actor/Ingredient.h"
#include "Actor/Sandwich.h"

APartTimerAIController::APartTimerAIController()
{
	
}

void APartTimerAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

bool APartTimerAIController::CheckIfUncookedIngrOnCuttingBoard()
{
	if (IsChopping) {
		return true;
	}

	TArray<AActor*> CuttingBoards;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BP_CuttingBoard, CuttingBoards);

	for (auto Actor : CuttingBoards) {
		ACuttingBoard* CuttingBoard = Cast<ACuttingBoard>(Actor);

		if (CuttingBoard->IsIngredientOn && !CuttingBoard->PlacedIngredient->IsCooked()) {
			HasUnCookedIngrCuttingBoard = CuttingBoard;
			return true;
		}
	}

	return false;
}

void APartTimerAIController::ChopIngrOnCuttingBoard()
{
	AIngredient* IngrOnCuttingBoard = HasUnCookedIngrCuttingBoard->PlacedIngredient;
	FTimerHandle ChopTimerHandler;

	bool IsIngrCooked = IngrOnCuttingBoard->IsCooked();

	if (!IsIngrCooked) {
		HasUnCookedIngrCuttingBoard->Chop();

		if (!GetWorld()->GetTimerManager().IsTimerActive(ChopTimerHandler)) {
			GetWorld()->GetTimerManager().SetTimer(
				ChopTimerHandler,
				this,
				&APartTimerAIController::ChopIngrOnCuttingBoard,
				0.5f,
				false);
		}
	}
	else {
		GetWorld()->GetTimerManager().ClearTimer(ChopTimerHandler);
		IsChopping = false;
	}
}

bool APartTimerAIController::CheckIfCompleteSandwichOnTable()
{
	TArray<AActor*> Tables;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BP_Table, Tables);

	for (auto Actor : Tables) {
		ATable* Table = Cast<ATable>(Actor);

		if (Table->PlacedActor && Table->PlacedActor->GetClass()->IsChildOf(ASandwich::StaticClass())) {
			ASandwich* Sandwich = Cast<ASandwich>(Table->PlacedActor);

			if (Sandwich->IsCompleteSandwich()) {
				HasCompleteSandwichTable = Table;
				return true;
			}
		}
	}

	return false;
}
