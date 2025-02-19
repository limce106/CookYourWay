// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DiningTable.h"
#include <Kismet/GameplayStatics.h>
#include "Reuben.h"
#include "PartTimer.h"

ADiningTable::ADiningTable()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADiningTable::BeginPlay()
{
	Super::BeginPlay();

}

void ADiningTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADiningTable::DiningTableInteraction()
{
	AReuben* Reuben = Cast<AReuben>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (SeatedCustomer) {
		bool SucceessGive = Reuben->TryGiveSomething(SeatedCustomer);

		if (!SucceessGive) {
			SeatedCustomer->TrySetComment();
		}
	}
}

void ADiningTable::PutFoodOn(AActor* HoldingCharacter, AActor* Food)
{
	if (IsActorOn) {
		return;
	}

	if (HoldingCharacter->GetClass()->IsChildOf(AReuben::StaticClass())) {
		AReuben* Reuben = Cast<AReuben>(UGameplayStatics::GetPlayerPawn(this, 0));
		Reuben->PutDownActor();
	}
	else if (HoldingCharacter->GetClass()->IsChildOf(APartTimer::StaticClass())) {
		APartTimer* PartTimer = Cast<APartTimer>(UGameplayStatics::GetActorOfClass(GetWorld(), APartTimer::StaticClass()));
		PartTimer->PutDownActor();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Can't Find Holding Food Character!"));
	}

	FVector ActorLocation = GetActorLocation();
	ActorLocation.Z += 65.0f;
	Food->SetActorLocation(ActorLocation);
	Food->SetActorRotation(GetActorRotation());

	IsActorOn = true;
	PlacedActor = Food;
}

void ADiningTable::DestroyFoodOnDiningTable()
{
	if (IsActorOn) {
		if (PlacedActor->GetClass()->IsChildOf(ASandwich::StaticClass())) {
			ASandwich* Sandwich = Cast<ASandwich>(PlacedActor);
			Sandwich->DestroySandwich();
		}
		else {
			PlacedActor->Destroy();
		}
		IsActorOn = false;
		PlacedActor = NULL;
	}
}

