// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PartTimer.h"

APartTimer::APartTimer()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APartTimer::BeginPlay()
{
	Super::BeginPlay();
	
}

void APartTimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APartTimer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

