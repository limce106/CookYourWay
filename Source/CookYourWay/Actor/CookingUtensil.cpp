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

void ACookingUtensil::Interact_Implementation()
{
	isInteractionSuccess = false;

	// 조리도구 위에 조리 완료된 재료가 있다면 재료를 샌드위치 위로 올린다.
	if (IsIngredientOn && PlacedIngredient->IsCooked() && Reuben->IsHold)
	{
		if (ASandwich* Sandwich = Cast<ASandwich>(Reuben->HeldActor))
		{
			Sandwich->AddIngredient(PlacedIngredient);

			IsIngredientOn = false;
			PlacedIngredient = NULL;

			BP_CookRateWidget->SetVisibility(ESlateVisibility::Hidden);

			USoundBase* LoadedSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Assets/Sound/SoundAsset/SFX_Stack.SFX_Stack"));
			UGameplayStatics::PlaySoundAtLocation(this, LoadedSound, GetActorLocation());

			isInteractionSuccess = true;
		}
	}
}