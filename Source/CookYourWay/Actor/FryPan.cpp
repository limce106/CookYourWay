// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FryPan.h"
#include "Ingredient.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/VillageManagerSystem.h"
#include "Reuben.h"
#include "Particles/ParticleSystemComponent.h"

void AFryPan::BeginPlay()
{
	Super::BeginPlay();

	ParticleSystemComponent = Cast<UParticleSystemComponent>(FindComponentByClass(UParticleSystemComponent::StaticClass()));
	ParticleSystemComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	ParticleSystemComponent->SetWorldScale3D(FVector(0.6f, 0.6f, 0.6f));
	ParticleSystemComponent->bAutoActivate = false;
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

	FVector IngrLocation = GetActorLocation();
	IngrLocation.Z += 140.0f;
	Ingr->SetActorLocation(IngrLocation);

	IsFrying = true;

	UParticleSystem* SteamParticle = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/Effect/P_Steam_Lit.P_Steam_Lit"));
	ParticleSystemComponent->SetTemplate(SteamParticle);
	ParticleSystemComponent->ActivateSystem();
}

void AFryPan::Fry()
{
	PlacedIngredient->CurCookRate += GetOneCookIncreasement();
	BP_CookRateWidget->CookRate += GetOneCookIncreasement();

	// 최대 조리 정도에서 5초 더 구워지면 태움 처리
	if (PlacedIngredient->CurCookRate > PlacedIngredient->MaxCookRate + (GetOneCookIncreasement() * 5) && !PlacedIngredient->IsBurn) {
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

	UParticleSystem* FireParticle = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/Effect/P_Fire.P_Fire"));
	ParticleSystemComponent->SetTemplate(FireParticle);
}
