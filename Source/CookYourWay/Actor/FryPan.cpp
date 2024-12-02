// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FryPan.h"
#include "Ingredient.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/VillageManagerSystem.h"

void AFryPan::BeginPlay()
{
	Super::BeginPlay();

	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();
}

float AFryPan::GetOneCookIncreasement()
{
	return (1.0f / 10.0f);
}

void AFryPan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsIngredientOn && IsFrying && VillageManagerSystem->DelayWithDeltaTime(2.0f, DeltaTime)) {
		Fry();
	}
}

void AFryPan::PutIngrOn(AIngredient* Ingr)
{
	Super::PutIngrOn(Ingr);
	IsFrying = true;
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
