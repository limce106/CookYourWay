// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CuttingBoard.h"
#include "Ingredient.h"
#include <Kismet/GameplayStatics.h>
#include "Reuben.h"
#include "Sound/SoundBase.h"

void ACuttingBoard::BeginPlay()
{
	Super::BeginPlay();

}

float ACuttingBoard::GetCookIncreasement()
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
	IngrLocation.Z += 52.0f;
	Ingr->SetActorLocation(IngrLocation);

	SetIngredientShrinkRate();
}

void ACuttingBoard::Chop()
{
	if (IsIngredientOn && PlacedIngredient->CurCookRate < PlacedIngredient->MaxCookRate) {
		PlacedIngredient->CurCookRate += GetCookIncreasement();
		BP_CookRateWidget->CookRate += GetCookIncreasement();


		if (IngrStaticMesh) {
			IngrStaticMesh->SetWorldScale3D(IngrStaticMesh->GetComponentScale() - IngredientShrinkRate);
			PlacedIngredient->SetPivotCenter();
		}
	}
	USoundBase* MetaSoundAsset = LoadObject<USoundBase>(nullptr,TEXT("/Game/Assets/Sound/MSS/SFX_Chop.SFX_Chop"));	
	UGameplayStatics::PlaySound2D(this, MetaSoundAsset);
}

void ACuttingBoard::Interact_Implementation()
{
	Super::Interact_Implementation();

	if (isInteractionSuccess)
		return;

	if (!IsIngredientOn && Reuben->IsHold)
	{
		if (AIngredient* Ingredient = Cast<AIngredient>(Reuben->HeldActor))
		{
			if (Ingredient->CurIngrData.IngrType == "Filling")
			{
				PutIngrOn(Ingredient);
			}
		}
	}
}
