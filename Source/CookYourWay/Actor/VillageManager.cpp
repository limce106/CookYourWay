// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VillageManager.h"
#include "PlayerBistro.h"
#include "Competitor.h"
#include "Store.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>

AVillageManager::AVillageManager()
{
	PrimaryActorTick.bCanEverTick = true;

	int AreaNum = 0;
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 6; j++) {
			float LocX = -1700 + 600.0f * (j - 1);
			float LocY = -2100.0f + 600.0f * (i - 1);
			FVector Loc = FVector(LocX, LocY, 0.0f);
			AreaLocMap.Add(++AreaNum, Loc);
		}
	}
}

void AVillageManager::Init()
{
	APlayerBistro* PlayerBistro = GetWorld()->SpawnActor<APlayerBistro>(BP_PlayerBistro, *AreaLocMap.Find(16), FRotator::ZeroRotator);

	ACompetitor* Competitor1 = GetWorld()->SpawnActor<ACompetitor>(BP_Competitor, *AreaLocMap.Find(5), FRotator::ZeroRotator);
	ACompetitor* Competitor2 = GetWorld()->SpawnActor<ACompetitor>(BP_Competitor, *AreaLocMap.Find(7), FRotator::ZeroRotator);
	ACompetitor* Competitor3 = GetWorld()->SpawnActor<ACompetitor>(BP_Competitor, *AreaLocMap.Find(11), FRotator::ZeroRotator);
	ACompetitor* Competitor4 = GetWorld()->SpawnActor<ACompetitor>(BP_Competitor, *AreaLocMap.Find(14), FRotator::ZeroRotator);
	ACompetitor* Competitor5 = GetWorld()->SpawnActor<ACompetitor>(BP_Competitor, *AreaLocMap.Find(18), FRotator::ZeroRotator);
	ACompetitor* Competitor6 = GetWorld()->SpawnActor<ACompetitor>(BP_Competitor, *AreaLocMap.Find(21), FRotator::ZeroRotator);

	AStore* Store1 = GetWorld()->SpawnActor<AStore>(BP_Store, *AreaLocMap.Find(2), FRotator::ZeroRotator);
	AStore* Store2 = GetWorld()->SpawnActor<AStore>(BP_Store, *AreaLocMap.Find(10), FRotator::ZeroRotator);
	AStore* Store3 = GetWorld()->SpawnActor<AStore>(BP_Store, *AreaLocMap.Find(23), FRotator::ZeroRotator);

	SetAllCustTastes();
}

void AVillageManager::BeginPlay()
{
	Super::BeginPlay();

	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();
	Init();
}

void AVillageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString AVillageManager::GetRandomCustName()
{
	int32 RandomIdx = UKismetMathLibrary::RandomIntegerInRange(0, CustomerNames.Num() - 1);
	FString RandomCustName = CustomerNames[RandomIdx];
	return RandomCustName;
}

TArray<int32> AVillageManager::GetRandomTaste()
{
	TArray<int32> Taste;

	// 임의로 "레벨 상관없이" 속재료는 항상 3개를 선택하도록 함
	/*for (int i = 0; i < 3; i++) {
		int FillingIndex = UKismetMathLibrary::RandomIntegerInRange(0, IngredientManagerSystem->FillingRows.Num() - 1);
		Taste.Add(FillingIndex);
	}

	int MeatIndex = UKismetMathLibrary::RandomIntegerInRange(0, IngredientManagerSystem->MeatRows.Num() - 1);
	Taste.Add(MeatIndex);

	int SauceIndex = UKismetMathLibrary::RandomIntegerInRange(0, IngredientManagerSystem->SauceRows.Num() - 1);
	Taste.Add(SauceIndex);*/

	// 테스트
	Taste.Add(12);
	Taste.Add(12);
	//

	return Taste;
}

void AVillageManager::SetAllCustTastes()
{
	for (int i = 0; i < CustomerNames.Num(); i++) {
		CustNameToTasteMap.Add(CustomerNames[i], GetRandomTaste());
	}
}

TArray<int32> AVillageManager::GetCustTaste(FString CustName)
{
	TArray<int32> Taste = *CustNameToTasteMap.Find(CustName);
	return Taste;
}
