// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CookingUtensil.h"

// Sets default values
ACookingUtensil::ACookingUtensil()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACookingUtensil::BeginPlay()
{
	Super::BeginPlay();
	
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

