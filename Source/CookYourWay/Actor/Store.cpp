// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Store.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/VillageManagerSystem.h"
#include <Kismet/KismetMathLibrary.h>

AStore::AStore()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AStore::Init()
{
	USceneComponent* SceneComp = FindComponentByClass<USceneComponent>();
	TArray<USceneComponent*> ChildComponents;
	SceneComp->GetChildrenComponents(true, ChildComponents);

	for (auto Component : ChildComponents) {
		if (Component->GetClass() == UStaticMeshComponent::StaticClass()) {
			StaticMesh = Cast<UStaticMeshComponent>(Component);
			break;
		}
	}

	SetStoreMesh();
	CreateCustomer();
}

void AStore::BeginPlay()
{
	Super::BeginPlay();

	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();
}

TArray<FString> AStore::GetStoreCustNames()
{
	TArray<FString> CustNames;
	if (CurStoreData.StoreCust1 != "-1") {
		CustNames.Add(CurStoreData.StoreCust1);
	}
	if (CurStoreData.StoreCust2 != "-1") {
		CustNames.Add(CurStoreData.StoreCust2);
	}
	if (CurStoreData.StoreCust3 != "-1") {
		CustNames.Add(CurStoreData.StoreCust3);
	}
	return CustNames;
}

void AStore::InitializeStoreData(const FStoreData& StoreData)
{
	CurStoreData = StoreData;
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

void AStore::SetStoreMesh()
{
	UStaticMesh* StoreMesh = CurStoreData.StoreModel;
	StaticMesh->SetStaticMesh(StoreMesh);
}

void AStore::CreateCustomer()
{
	int32 CustNameIdx = UKismetMathLibrary::RandomIntegerInRange(0, GetStoreCustNames().Num() - 1);
	FString SpawnCustName = GetStoreCustNames()[CustNameIdx];

	FVector CustomerLocation = FVector(GetActorLocation().X, GetActorLocation().Y + 250.0, 95.0f);
	ACustomer* Customer = CustomerSpawnFactory::SpawnCustomer(GetWorld(), BP_Customer, CustomerLocation, FRotator(0.0f, 90.0f, 0.0f), SpawnCustName);
}