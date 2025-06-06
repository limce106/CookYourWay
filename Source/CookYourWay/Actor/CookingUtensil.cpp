// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CookingUtensil.h"
#include <Kismet/GameplayStatics.h>
#include "Ingredient.h"
#include "Reuben.h"

ACookingUtensil::ACookingUtensil()
{
	PrimaryActorTick.bCanEverTick = true;

	
}

void ACookingUtensil::BeginPlay()
{
	Super::BeginPlay();
	
	Reuben = Cast<AReuben>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void ACookingUtensil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ACookingUtensil::GetCookIncreasement()
{
	UE_LOG(LogTemp, Error, TEXT("Override the (GetOneCookIncreasement) function!"));

	return 0.0f;
}

void ACookingUtensil::PutIngrOn(AIngredient* Ingr)
{
	Reuben->PutDownActor();
	Ingr->SetActorRotation(GetActorRotation());

	IsIngredientOn = true;
	PlacedIngredient = Ingr;

	BP_CookRateWidget->CookRate = PlacedIngredient->CurCookRate;
	BP_CookRateWidget->SetVisibility(ESlateVisibility::Visible);
}

void ACookingUtensil::PickUpIngr()
{
	if (!IsIngredientOn)
		return;

	if (!Reuben->IsHold) {
		Reuben->HoldActor(PlacedIngredient);
	}
	else if (Reuben->IsHold && Reuben->HeldActor->GetClass()->IsChildOf(ASandwich::StaticClass())) {
		ASandwich* HoldingSandwich = Cast<ASandwich>(Reuben->HeldActor);
		HoldingSandwich->AddIngredient(PlacedIngredient);
	}

	IsIngredientOn = false;
	PlacedIngredient = NULL;

	BP_CookRateWidget->SetVisibility(ESlateVisibility::Hidden);
}

bool ACookingUtensil::CommonCookingUtensilInteraction()
{
	if (!Reuben->IsHold) {
		if (IsIngredientOn) {
			PickUpIngr();
			return true;
		}
	}
	else if (Reuben->GetHeldActorClass()->IsChildOf(ASandwich::StaticClass())) {
		if (IsIngredientOn && PlacedIngredient->IsCooked()) {
			PickUpIngr();
		}
		return true;
	}
	
	return false;
}
