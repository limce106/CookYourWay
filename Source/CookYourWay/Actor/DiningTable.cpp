// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/DiningTable.h"
#include <Kismet/GameplayStatics.h>
#include "Reuben.h"

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

