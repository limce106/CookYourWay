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
	Reuben->PutDownActor();

	FVector ActorLocation = GetActorLocation();
	ActorLocation.Z += 65.0f;
	Actor->SetActorLocation(ActorLocation);
	Actor->SetActorRotation(GetActorRotation());

	IsActorOn = true;
	PlacedActor = Actor;

	FRotator PlacedActorRotation = Reuben->GetActorRotation();
	PlacedActor->SetActorRotation(PlacedActorRotation);
}

void ATable::PickUpActor()
{
	if (!IsActorOn || Reuben->IsHold) {
		return;
	}

	Reuben->HoldActor(PlacedActor);

	IsActorOn = false;
	PlacedActor = NULL;
}

