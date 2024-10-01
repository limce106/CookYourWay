// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Table.h"

ATable::ATable()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ATable::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATable::PutActorOn(AActor* Actor)
{
	IsActorOn = true;
	PlacedActor = Actor;
}

void ATable::PickUpActor()
{
	IsActorOn = false;
	PlacedActor = NULL;
}

