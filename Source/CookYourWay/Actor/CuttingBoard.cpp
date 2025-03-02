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
	return (float)(1.0f / CutCount);
}

void ACuttingBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACuttingBoard::SetIngredientShrinkRate()
{
	IngrStaticMesh = Cast<UStaticMeshComponent>(PlacedIngredient->FindComponentByClass(UStaticMeshComponent::StaticClass()));
	IngredientShrinkRate = (float)((IngrStaticMesh->GetComponentScale().X - 1.5f) / CutCount);
}

void ACuttingBoard::PutIngrOn(AIngredient* Ingr)
{
	Super::PutIngrOn(Ingr);

	FVector IngrLocation = GetActorLocation();
	IngrLocation.X -= 15.0f;
	IngrLocation.Z += 110.0f;
	Ingr->SetActorLocation(IngrLocation);

	SetIngredientShrinkRate();
}

void ACuttingBoard::Chop()
{
	if (IsIngredientOn && PlacedIngredient->CurCookRate < PlacedIngredient->MaxCookRate) {
		PlacedIngredient->CurCookRate += GetOneCookIncreasement();
		BP_CookRateWidget->CookRate += GetOneCookIncreasement();

		if (IngrStaticMesh) {
			IngrStaticMesh->SetWorldScale3D(IngrStaticMesh->GetComponentScale() - IngredientShrinkRate);
			PlacedIngredient->SetPivotCenter();
		}
	}
}

void ACuttingBoard::CuttingBoardInteraction()
{
	bool InteractionSuccess = CommonCookingUtensilInteraction();
	if (InteractionSuccess) {
		return;
	}

	if (Reuben->IsHold && Reuben->GetHeldActorClass()->IsChildOf(AIngredient::StaticClass())) {
		AIngredient* HoldingIngr = Cast<AIngredient>(Reuben->HeldActor);
		if (HoldingIngr->CurIngrData.IngrType == "Filling" && !this->IsIngredientOn) {
			PutIngrOn(HoldingIngr);
		}
	}
}
