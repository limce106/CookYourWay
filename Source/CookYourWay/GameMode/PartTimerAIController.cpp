// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/PartTimerAIController.h"
#include "BehaviorTree//BlackboardComponent.h"
#include "BehaviorTree//BehaviorTree.h"
#include "BehaviorTree//BlackboardData.h"
#include <Kismet/GameplayStatics.h>
#include "Actor/Ingredient.h"

const FName APartTimerAIController::HasUncookedIngrOnCuttingBoard(TEXT("HasUncookedIngrOnCuttingBoard"));
const FName APartTimerAIController::IsCompletedSandwichExist(TEXT("IsCompletedSandwichExist"));
const FName APartTimerAIController::MovePos(TEXT("MovePos"));

APartTimerAIController::APartTimerAIController()
{
	
}

void APartTimerAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

bool APartTimerAIController::CheckIfUncookedIngrOnCuttingBoard()
{
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

	HasUnCookedIngrCuttingBoard->Chop();

	/*while (!IngrOnCuttingBoard->IsCooked()) {
		GetWorld()->GetTimerManager().SetTimer(ChopTimerHandler, FTimerDelegate::CreateLambda([=]()
			{
				HasUnCookedIngrCuttingBoard->Chop();
			}), 0.5f, false);
	}*/
}
