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

float ACookingUtensil::GetOneCookIncreasement()
{
	UE_LOG(LogTemp, Error, TEXT("Override the (GetOneCookIncreasement) function!"));

	return 0.0f;
}

void ACookingUtensil::PutIngrOn(AIngredient* Ingr)
{
	Reuben->PutDownActor();

	FVector IngrLocation = GetActorLocation();
	IngrLocation.Z += 5.0f;
	Ingr->SetActorLocation(IngrLocation);
	Ingr->SetActorRotation(GetActorRotation());

	IsIngredientOn = true;
	PlacedIngredient = Ingr;

	BP_CookRateWidget->CookRate = PlacedIngredient->CurCookRate;
	BP_CookRateWidget->SetVisibility(ESlateVisibility::Visible);
}

void ACookingUtensil::PickUpIngr()
{
	IsIngredientOn = false;
	PlacedIngredient = NULL;

	BP_CookRateWidget->SetVisibility(ESlateVisibility::Hidden);
}
