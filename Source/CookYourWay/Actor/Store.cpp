// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Store.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/VillageManagerSystem.h"
#include <Kismet/KismetMathLibrary.h>
#include <Component/NewsEffectComponent.h>

AStore::AStore()
{
	PrimaryActorTick.bCanEverTick = true;

	NewsEffectComponent = CreateDefaultSubobject<UNewsEffectComponent>(TEXT("NewsEffectComponent"));
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
	SetStoreCustName();
	SetSpawnCustDelayTime();
	CreateCustomer();
}

void AStore::BeginPlay()
{
	Super::BeginPlay();

	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();
}

void AStore::SetStoreCustName()
{
	if (CurStoreTableData.StoreCust1 != "-1") {
		StoreCustName.Add(CurStoreTableData.StoreCust1);
	}
	if (CurStoreTableData.StoreCust2 != "-1") {
		StoreCustName.Add(CurStoreTableData.StoreCust2);
	}
	if (CurStoreTableData.StoreCust3 != "-1") {
		StoreCustName.Add(CurStoreTableData.StoreCust3);
	}
}

void AStore::SetSpawnCustDelayTime()
{
	bool IsNewsRelative = NewsEffectComponent->CurNewsKeyWord == CurStoreTableData.StoreName;
	if (IsNewsRelative) {
		SpawnCustMin = NewsEffectComponent->IncreaseSpawnCustMin;
		SpawnCustMax = NewsEffectComponent->IncreaseSpawnCustMax;
	}
	else {
		SpawnCustMin = 5;
		SpawnCustMax = 7;
	}
	SpawnDelayTime = SpawnCustMin;
}

void AStore::InitializeStoreTableData(int32 StoreAreaID, FStoreTable StoreTableData)
{
	AreaID = StoreAreaID;
	CurStoreTableData = StoreTableData;
}

void AStore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DelayWithDeltaTime(SpawnDelayTime, DeltaTime)) {
		CreateCustomer();
		SpawnDelayTime = UKismetMathLibrary::RandomIntegerInRange(SpawnCustMin, SpawnCustMax);
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
	UStaticMesh* StoreMesh = CurStoreTableData.StoreModel;
	StaticMesh->SetStaticMesh(StoreMesh);
}

void AStore::CreateCustomer()
{
	FString SpawnCustName = GetRandomCustName();

	FVector CustomerLocation = FVector(GetActorLocation().X, GetActorLocation().Y + 250.0, 95.0f);
	ACustomer* Customer = CustomerSpawnFactory::SpawnCustomer(GetWorld(), BP_Customer, CustomerLocation, FRotator(0.0f, 90.0f, 0.0f), SpawnCustName);
}

FString AStore::GetRandomCustName()
{
	bool IsNewsRelative = StoreCustName.Contains(NewsEffectComponent->CurNewsKeyWord);
	int32 CustNum = StoreCustName.Num();

	TArray<float> SpawnCustPercent;
	if (NewsEffectComponent->CurNewsEffect == "CustDec" && IsNewsRelative) {
		SpawnCustPercent = NewsEffectComponent->GetDecSpawnCustPercent(StoreCustName);
	}
	else if (NewsEffectComponent->CurNewsEffect == "CustInc" && IsNewsRelative) {
		SpawnCustPercent = NewsEffectComponent->GetIncSpawnCustPercent(StoreCustName);
	}
	else {
		float CustPercent = 1 / CustNum;

		for (int i = 0; i < CustNum; i++) {
			SpawnCustPercent.Add(CustPercent);
		}
	}

	float Probability = FMath::FRand();
	float ProbRange = 0.0f;

	for (int i = 0; i < CustNum; i++) {
		ProbRange += SpawnCustPercent[i];
		if (Probability > ProbRange) {
			continue;
		}
		else {
			return StoreCustName[i];
		}
	}
}