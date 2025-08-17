// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Table.h"
#include <Kismet/GameplayStatics.h>
#include "Reuben.h"
#include "PartTimer.h"

void ATable::BeginPlay()
{
	Super::BeginPlay();

	Reuben = Cast<AReuben>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void ATable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATable::Interact_Implementation()
{
	if (!Reuben->HeldActor && IsActorOn && PlacedActor->GetClass()->ImplementsInterface(UHoldable::StaticClass()))
	{
		IHoldable::Execute_OnPickUp(PlacedActor);

		IsActorOn = false;
		PlacedActor = nullptr;
	}
	else if (!IsActorOn && Reuben->HeldActor && Reuben->HeldActor->GetClass()->ImplementsInterface(UHoldable::StaticClass()))
	{
		IsActorOn = true;
		PlacedActor = Reuben->HeldActor;

		IHoldable::Execute_OnPutDown(Reuben->HeldActor, this);
	}
}

