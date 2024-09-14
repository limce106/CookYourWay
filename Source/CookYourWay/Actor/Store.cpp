// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Store.h"
#include "Customer.h"

AStore::AStore()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AStore::BeginPlay()
{
	Super::BeginPlay();

}

void AStore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DelayWithDeltaTime(2, DeltaTime)) {
		CreateCustomer();
	}
}

bool AStore::DelayWithDeltaTime(float DelayTime, float DeltaSeconds)
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

void AStore::CreateCustomer()
{
	FVector CustomerLocation = FVector(GetActorLocation().X, GetActorLocation().Y + 220.0, 5.0f);
	ACustomer* Customer = GetWorld()->SpawnActor<ACustomer>(BP_Customer, CustomerLocation, GetActorRotation());
}