// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FryPan.h"

float AFryPan::GetOneCookIncreasement()
{
	return (1.0f / 20.0f);
}

void AFryPan::Tick(float DeltaTime)
{
	if (IsIngredientOn && IsFrying) {
		Fry();
	}
}

void AFryPan::Fry()
{
	PlacedIngredient->CurCookRate += GetOneCookIncreasement();

	// 최대 조리 정도에서 5초 더 구워지면 태움 처리
	if (PlacedIngredient->CurCookRate > PlacedIngredient->MaxCookRate + (GetOneCookIncreasement() * 5)) {
		IsFrying = false;
		PlacedIngredient->IsBurn = true;
	}
}
