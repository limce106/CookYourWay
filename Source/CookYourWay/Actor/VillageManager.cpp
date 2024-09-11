// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VillageManager.h"
#include "PlayerBistro.h"
#include "Competitor.h"
#include "Store.h"

AVillageManager::AVillageManager()
{
	PrimaryActorTick.bCanEverTick = true;

	for (int i = 0; i < 4; i++) {
		int C = i + 1;
		for (int j = 0; j < 6; j++) {
			int R = j + 1;
			FVector Loc = FVector((-1600.0f + 800.0f * j), (-2100.0f + 600.0f * i), 0);
			AreaLocMap.Add(C * R, Loc);
		}
	}
}

void AVillageManager::Init()
{
	APlayerBistro* PlayerBistro = GetWorld()->SpawnActor<APlayerBistro>(BP_PlayerBistro, *AreaLocMap.Find(16), FRotator(0.0f, 0.0f, 180.0f));

	ACompetitor* Competitor1 = GetWorld()->SpawnActor<ACompetitor>(BP_Competitor, *AreaLocMap.Find(5), FRotator(0.0f, 0.0f, 180.0f));
	ACompetitor* Competitor2 = GetWorld()->SpawnActor<ACompetitor>(BP_Competitor, *AreaLocMap.Find(7), FRotator(0.0f, 0.0f, 180.0f));
	ACompetitor* Competitor3 = GetWorld()->SpawnActor<ACompetitor>(BP_Competitor, *AreaLocMap.Find(11), FRotator(0.0f, 0.0f, 180.0f));
	ACompetitor* Competitor4 = GetWorld()->SpawnActor<ACompetitor>(BP_Competitor, *AreaLocMap.Find(14), FRotator(0.0f, 0.0f, 180.0f));
	ACompetitor* Competitor5 = GetWorld()->SpawnActor<ACompetitor>(BP_Competitor, *AreaLocMap.Find(18), FRotator(0.0f, 0.0f, 180.0f));
	ACompetitor* Competitor6 = GetWorld()->SpawnActor<ACompetitor>(BP_Competitor, *AreaLocMap.Find(21), FRotator(0.0f, 0.0f, 180.0f));

	AStore* Store1 = GetWorld()->SpawnActor<AStore>(BP_Store, *AreaLocMap.Find(2), FRotator(0.0f, 0.0f, 180.0f));
	AStore* Store2 = GetWorld()->SpawnActor<AStore>(BP_Store, *AreaLocMap.Find(10), FRotator(0.0f, 0.0f, 180.0f));
	AStore* Store3 = GetWorld()->SpawnActor<AStore>(BP_Store, *AreaLocMap.Find(23), FRotator(0.0f, 0.0f, 180.0f));
}

void AVillageManager::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

void AVillageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}