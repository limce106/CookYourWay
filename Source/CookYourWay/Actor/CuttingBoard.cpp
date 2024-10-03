// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CuttingBoard.h"
#include "Ingredient.h"

void ACuttingBoard::BeginPlay()
{
	Super::BeginPlay();
}

float ACuttingBoard::GetOneCookIncreasement()
{
	return (1.0f / 15.0f);
}

void ACuttingBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACuttingBoard::Chop()
{
	if (IsIngredientOn && PlacedIngredient->CurCookRate < PlacedIngredient->MaxCookRate) {
		PlacedIngredient->CurCookRate += GetOneCookIncreasement();
	}
}
