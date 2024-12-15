// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Store.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/VillageManagerSystem.h"

AStore::AStore()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AStore::BeginPlay()
{
	Super::BeginPlay();

	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();
}

void AStore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (VillageManagerSystem->DelayWithDeltaTime(SpawnDelayTime, DeltaTime)) {
		CreateCustomer();
	}
}

void AStore::CreateCustomer()
{
	FVector CustomerLocation = FVector(GetActorLocation().X, GetActorLocation().Y + 250.0, 95.0f);
	ACustomer* Customer = GetWorld()->SpawnActor<ACustomer>(BP_Customer, CustomerLocation, FRotator(0.0f, 90.0f, 0.0f));
}