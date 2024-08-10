// Fill out your copyright notice in the Description page of Project Settings.


#include "PastaYourWay/Actor/Store.h"

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

	if (DelayWithDeltaTime(30, DeltaTime)) {
		CreateCustomer();
	}
}

bool AStore::DelayWithDeltaTime(float DelayTime, float DeltaSeconds)
{
	if (TempTime > DelayTime) {
		TempTime = 0;
		return true;
	}
	else {
		TempTime += DeltaSeconds;
		return false;
	}
}

void AStore::CreateCustomer()
{
	Customer = GetWorld()->SpawnActor<ACustomer>(BP_CustomerClass, this->GetActorLocation(), this->GetActorRotation());

}

