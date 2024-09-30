// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CuttingBoard.h"

float ACuttingBoard::GetOneCookIncreasement()
{
	return (1.0f / 15.0f);
}

void ACuttingBoard::Chop()
{
	if (CurCookRate < MaxCookRate) {
		CurCookRate += GetOneCookIncreasement();
	}
}
