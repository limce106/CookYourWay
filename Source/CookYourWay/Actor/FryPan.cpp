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
	CurCookRate += GetOneCookIncreasement();

	// �ִ� ���� �������� 5�� �� �������� �¿� ó��
	if (CurCookRate > MaxCookRate + (GetOneCookIncreasement() * 5)) {
		IsFrying = false;
		PlacedIngredient->IsBurn = true;
	}
}
