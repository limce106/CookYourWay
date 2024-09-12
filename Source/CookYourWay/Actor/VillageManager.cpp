// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VillageManager.h"
#include "PlayerBistro.h"
#include "Competitor.h"
#include "Store.h"

AVillageManager::AVillageManager()
{
	PrimaryActorTick.bCanEverTick = true;

	int AreaNum = 0;
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 6; j++) {
			float LocX = 2400.0f - 800.0f * (j - 1);
			float LocY = -300.0f - 600.0f * (i - 1);
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
}

void AVillageManager::BeginPlay()
{
	Super::BeginPlay();

	//Init();
}

void AVillageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}