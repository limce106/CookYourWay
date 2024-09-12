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

	if (DelayWithDeltaTime(5, DeltaTime)) {
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
	FVector CustomerLocation = FVector(GetActorLocation().X, GetActorLocation().Y + 200.0, GetActorLocation().Z);
	ACustomer* Customer = GetWorld()->SpawnActor<ACustomer>(BP_Customer, CustomerLocation, GetActorRotation());
	Customer->CustName = "Amy"; // 임의로 테스트를 위해 설정
	Customer->SetSkeletalMesh();
}