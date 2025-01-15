// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Store.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/VillageManagerSystem.h"
#include <Kismet/KismetMathLibrary.h>

AStore::AStore()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AStore::BeginPlay()
{
	Super::BeginPlay();

	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();
	CreateCustomer();
}

void AStore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DelayWithDeltaTime(SpawnDelayTime, DeltaTime)) {
		CreateCustomer();
		SpawnDelayTime = UKismetMathLibrary::RandomIntegerInRange(5, 7);
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
	FVector CustomerLocation = FVector(GetActorLocation().X, GetActorLocation().Y + 250.0, 95.0f);
	ACustomer* Customer = GetWorld()->SpawnActor<ACustomer>(BP_Customer, CustomerLocation, FRotator(0.0f, 90.0f, 0.0f));
}