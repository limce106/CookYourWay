// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Table.h"
#include <Kismet/GameplayStatics.h>
#include "Reuben.h"

ATable::ATable()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ATable::BeginPlay()
{
	Super::BeginPlay();

	Reuben = Cast<AReuben>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void ATable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATable::PutActorOn(AActor* Actor)
{
	Reuben->DetachActorFromSocket();

	FVector ActorLocation = GetActorLocation();
	ActorLocation.Z += 65.0f;
	Actor->SetActorLocation(ActorLocation);
	Actor->SetActorRotation(GetActorRotation());

	IsActorOn = true;
	PlacedActor = Actor;
}

void ATable::PickUpActor()
{
	Reuben->AttachToSocket(PlacedActor);

	IsActorOn = false;
	PlacedActor = NULL;
}

