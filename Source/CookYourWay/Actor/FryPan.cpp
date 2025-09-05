// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FryPan.h"
#include "Ingredient.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/VillageManagerSystem.h"
#include "Reuben.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"

void AFryPan::BeginPlay()
{
	Super::BeginPlay();

	ParticleSystemComponent = Cast<UParticleSystemComponent>(FindComponentByClass(UParticleSystemComponent::StaticClass()));
	ParticleSystemComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	ParticleSystemComponent->SetWorldScale3D(FVector(0.6f, 0.6f, 0.6f));
	ParticleSystemComponent->bAutoActivate = false;

	//Audio component
	FryingAudioComponent = NewObject<UAudioComponent>(this, TEXT("FryingAudio"));
	if (FryingAudioComponent)
	{
		FryingAudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		FryingAudioComponent->RegisterComponent();
		FryingAudioComponent->bAutoActivate = false;
		if (FrySound) FryingAudioComponent->SetSound(FrySound);
	}
	BurntAudioComponent = NewObject<UAudioComponent>(this, TEXT("BurntAudio"));
	if (BurntAudioComponent)
	{
		BurntAudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		BurntAudioComponent->RegisterComponent();
		BurntAudioComponent->bAutoActivate = false;
		if (BurntSound) BurntAudioComponent->SetSound(BurntSound);
	}
}

float AFryPan::GetCookIncreasement()
{
	return (1.0f / 10.0f);
	// 테스트
	//return (1.0f / 3.0f);
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
	IngrLocation.Z += 50.0f;
	Ingr->SetActorLocation(IngrLocation);

	IsFrying = true;

	UParticleSystem* SteamParticle = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/Effect/P_Steam_Lit.P_Steam_Lit"));
	ParticleSystemComponent->SetTemplate(SteamParticle);
	ParticleSystemComponent->ActivateSystem();

	//
	if (FryingAudioComponent)
	{
		FryingAudioComponent->Play();
	}

}

void AFryPan::Fry()
{
	if (PlacedIngredient->IsBurn) {
		return;
	}

	PlacedIngredient->CurCookRate += GetCookIncreasement();
	BP_CookRateWidget->CookRate += GetCookIncreasement();

	if (PlacedIngredient->CurCookRate == PlacedIngredient->MaxCookRate) {
		PlacedIngredient->AddBurntMaterialOverlay();
	}
	else if (PlacedIngredient->CurCookRate > PlacedIngredient->MaxCookRate) {
		OverCookedTime--;
		// 최대 조리 정도에서 3초 더 구워지면 태움 처리
		if (OverCookedTime == 0) {
			IsFrying = false;
			PlacedIngredientBurnt();
		}
	}
}

void AFryPan::PlacedIngredientBurnt()
{
	PlacedIngredient->IsBurn = true;
	PlacedIngredient->AddBurntMaterialOverlay();

	UParticleSystem* FireParticle = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/Effect/P_Fire.P_Fire"));
	ParticleSystemComponent->SetTemplate(FireParticle);

	if (BurntAudioComponent)
	{
		BurntAudioComponent->Play();
		FryingAudioComponent->SetVolumeMultiplier(0.6f);
	}
}

void AFryPan::Interact_Implementation()
{
	Super::Interact_Implementation();

	if (isInteractionSuccess)
	{
		ParticleSystemComponent->SetTemplate(nullptr);
		FryingAudioComponent->FadeOut(0.3f, 0.0f);
		BurntAudioComponent->FadeOut(0.3f, 0.0f);

		return;
	}

	if (!IsIngredientOn && Reuben->IsHold)
	{
		if (AIngredient* Ingredient = Cast<AIngredient>(Reuben->HeldActor))
		{
			if (Ingredient->CurIngrData.IngrType == "Meat")
			{
				PutIngrOn(Ingredient);
			}
		}
	}
}
