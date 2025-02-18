// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CuttingBoard.h"
#include "Ingredient.h"
#include <Kismet/GameplayStatics.h>
#include "Reuben.h"

void ACuttingBoard::BeginPlay()
{
	Super::BeginPlay();

}

float ACuttingBoard::GetOneCookIncreasement()
{
	return (1.0f / 15.0f);

	// Å×½ºÆ®
	// return (1.0f / 1.0f);
	//
}

void ACuttingBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACuttingBoard::PutIngrOn(AIngredient* Ingr)
{
	Super::PutIngrOn(Ingr);
}

void ACuttingBoard::Chop()
{
	if (IsIngredientOn && PlacedIngredient->CurCookRate < PlacedIngredient->MaxCookRate) {
		PlacedIngredient->CurCookRate += GetOneCookIncreasement();
		BP_CookRateWidget->CookRate += GetOneCookIncreasement();
	}
}

void ACuttingBoard::CuttingBoardInteraction()
{
	bool InteractionSuccess = CommonCookingUtensilInteraction();
	if (InteractionSuccess) {
		return;
	}

	if (Reuben->GetHeldActorClass()->IsChildOf(AIngredient::StaticClass())) {
		AIngredient* HoldingIngr = Cast<AIngredient>(Reuben->HeldActor);
		if (HoldingIngr->CurIngrData.IngrType == "Filling" && !this->IsIngredientOn) {
			PutIngrOn(HoldingIngr);
		}
	}
}
