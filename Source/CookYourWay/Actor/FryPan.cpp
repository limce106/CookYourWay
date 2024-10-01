// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FryPan.h"

void AFryPan::BeginPlay()
{
	Super::BeginPlay();
}

float AFryPan::GetOneCookIncreasement()
{
	return (1.0f / 20.0f);
}

void AFryPan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsIngredientOn && IsFrying) {
		Fry();
	}
}

void AFryPan::Fry()
{
	PlacedIngredient->CurCookRate += GetOneCookIncreasement();

	// �ִ� ���� �������� 5�� �� �������� �¿� ó��
	if (PlacedIngredient->CurCookRate > PlacedIngredient->MaxCookRate + (GetOneCookIncreasement() * 5)) {
		IsFrying = false;
		PlacedIngredient->IsBurn = true;
	}
}
