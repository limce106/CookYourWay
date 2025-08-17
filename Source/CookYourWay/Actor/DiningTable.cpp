// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DiningTable.h"
#include <Kismet/GameplayStatics.h>
#include "Reuben.h"
#include "PartTimer.h"
#include "Dessert.h"

ADiningTable::ADiningTable()
{
	PrimaryActorTick.bCanEverTick = true;

	Reuben = Cast<AReuben>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void ADiningTable::BeginPlay()
{
	Super::BeginPlay();

}

void ADiningTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADiningTable::DestroyFoodOnDiningTable()
{
	if (!IsActorOn)
		return;

	if (ASandwich* Sandwich = Cast<ASandwich>(PlacedActor))
	{
		Sandwich->DestroySandwich();
	}
	else
	{
		PlacedActor->Destroy();
	}

	IsActorOn = false;
	PlacedActor = nullptr;
}

void ADiningTable::Interact_Implementation()
{
	if (!SeatedCustomer) return;

	if (!Reuben->IsHold)
	{
		SeatedCustomer->TrySetComment();
		return;
	}

	if (ASandwich* Sandwich = Cast<ASandwich>(Reuben->HeldActor))
	{
		if (Sandwich->Ingredients.Num() == 0 || SeatedCustomer->IsEat)
		{
			SeatedCustomer->TrySetComment();
			return;
		}

		Reuben->GivenSandwichNum++;
		Sandwich->SetIngrWidgetVisibility(ESlateVisibility::Hidden);

		IHoldable::Execute_OnPutDown(Sandwich, this);
		SeatedCustomer->EatSandwich(Sandwich);

		IsActorOn = true;
		PlacedActor = Sandwich;
	}
	else if (ADessert* Dessert = Cast<ADessert>(Reuben->HeldActor))
	{
		if (!Dessert->IsCooked || !SeatedCustomer->CanGetDessert())
		{
			SeatedCustomer->TrySetComment();
			return;
		}

		IHoldable::Execute_OnPutDown(Dessert, this);
		SeatedCustomer->EatDessert();

		IsActorOn = true;
		PlacedActor = Dessert;
	}

	USoundBase* LoadedSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Assets/Sound/SoundAsset/SFX_PutIngredients.SFX_PutIngredients"));
	UGameplayStatics::PlaySoundAtLocation(this, LoadedSound, GetActorLocation());
}