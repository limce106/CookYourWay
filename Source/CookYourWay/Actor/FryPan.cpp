// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FryPan.h"
#include "Ingredient.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/VillageManagerSystem.h"
#include "Reuben.h"

void AFryPan::BeginPlay()
{
	Super::BeginPlay();

}

float AFryPan::GetOneCookIncreasement()
{
	// return (1.0f / 10.0f);
	// 테스트
	return (1.0f / 3.0f);
	//
}

void AFryPan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsIngredientOn && IsFrying && DelayWithDeltaTime(2.0f, DeltaTime)) {
		Fry();
	}
}

bool AFryPan::DelayWithDeltaTime(float DelayTime, float DeltaSeconds)
{
	if (TempDelayTime > DelayTime) {
		TempDelayTime = 0;
		return true;
	}
	else {
		TempDelayTime += DeltaSeconds;
		return false;
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
	BP_CookRateWidget->CookRate += GetOneCookIncreasement();

	// 최대 조리 정도에서 5초 더 구워지면 태움 처리
	if (PlacedIngredient->CurCookRate > PlacedIngredient->MaxCookRate + (GetOneCookIncreasement() * 5)) {
		IsFrying = false;
		PlacedIngredientBurnt();
	}
}

void AFryPan::FryPanInteraction()
{
	bool InteractionSuccess = CommonCookingUtensilInteraction();
	if (InteractionSuccess) {
		return;
	}

	if (Reuben->IsHold && Reuben->GetHeldActorClass()->IsChildOf(AIngredient::StaticClass())) {
		AIngredient* HoldingIngr = Cast<AIngredient>(Reuben->HeldActor);
		if (HoldingIngr->CurIngrData.IngrType == "Meat" && !this->IsIngredientOn) {
			PutIngrOn(HoldingIngr);
		}
	}
}

void AFryPan::PlacedIngredientBurnt()
{
	PlacedIngredient->IsBurn = true;
	PlacedIngredient->ReplaceBurntMeatMaterial();
}
