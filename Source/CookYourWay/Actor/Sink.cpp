// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Sink.h"
#include "Reuben.h"
#include <Kismet/GameplayStatics.h>

ASink::ASink()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASink::BeginPlay()
{
	Super::BeginPlay();
	
	Reuben = Cast<AReuben>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void ASink::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASink::Interact_Implementation()
{
	if (!Reuben->IsHold) {
		ASandwich* Sandwich = GetWorld()->SpawnActor<ASandwich>(BP_Sandwich, Reuben->GetActorLocation(), Reuben->GetActorRotation());
		Reuben->HoldActor(Sandwich);

		USoundBase* MetaSoundAsset = LoadObject<USoundBase>(nullptr, TEXT("/Game/Assets/Sound/MSS/SFX_Dish.SFX_Dish"));
		UGameplayStatics::PlaySoundAtLocation(this, MetaSoundAsset, Reuben->GetActorLocation());
	}
	else if (Reuben->IsHold) {
		if (AIngredient* Ingredient = Cast<AIngredient>(Reuben->HeldActor))
		{
			if (!Ingredient->IsCooked()) return;

			ASandwich* Sandwich = GetWorld()->SpawnActor<ASandwich>(BP_Sandwich, Reuben->GetActorLocation(), Reuben->GetActorRotation());
			Reuben->HoldActor(Sandwich);
			Sandwich->AddIngredient(Ingredient);
		}
	}
}

