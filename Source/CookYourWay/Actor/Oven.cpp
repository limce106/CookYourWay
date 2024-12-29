// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Oven.h"
#include "Reuben.h"
#include "Dessert.h"

void AOven::BeginPlay()
{
	Super::BeginPlay();
}

void AOven::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsIngredientOn && DelayWithDeltaTime(1.0f, DeltaTime)) {
		Bake();
	}
}

bool AOven::DelayWithDeltaTime(float DelayTime, float DeltaSeconds)
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

float AOven::GetOneCookIncreasement()
{
	return (1.0f / 6.0f);
}

void AOven::PutDessertIn(ADessert* Dessert)
{
	IsIngredientOn = true;
	IsBaking = true;
	Reuben->PutDownActor();
	Dessert->Destroy();
	BP_CookRateWidget->SetVisibility(ESlateVisibility::Visible);
}

void AOven::Bake()
{
	BP_CookRateWidget->CookRate += GetOneCookIncreasement();
}

bool AOven::IsCooked()
{
	if (BP_CookRateWidget->CookRate >= 1.0) {
		return true;
	}
	else {
		return false;
	}
}

void AOven::OvenInteraction()
{
	if (Reuben->IsHold && Reuben->GetHeldActorClass()->IsChildOf(ADessert::StaticClass())) {
		ADessert* HoldingDessert = Cast<ADessert>(Reuben->HeldActor);
		PutDessertIn(HoldingDessert);
	}
	else if (!Reuben->IsHold && IsCooked()) {
		ADessert* Dessert = GetWorld()->SpawnActor<ADessert>(BP_Dessert, Reuben->GetActorLocation(), Reuben->GetActorRotation());
		Dessert->IsCooked = true;

		Reuben->HoldActor(Dessert);
		BP_CookRateWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
