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
	APlayerBistro* PlayerBistro = GetWorld()->SpawnActor<APlayerBistro>(BP_PlayerBistro, *AreaLocMap.Find(VillageManagerSystem->PlayerBistroAreaID), FRotator::ZeroRotator);
	PlayerBistro->AreaID = VillageManagerSystem->PlayerBistroAreaID;

	for (int i = 0; i < VillageManagerSystem->CompetitorAreaID.Num(); i++) {
		int32 AreaID = VillageManagerSystem->CompetitorAreaID[i];
		ACompetitor* Competitor = GetWorld()->SpawnActor<ACompetitor>(BP_Competitor, *AreaLocMap.Find(AreaID), FRotator::ZeroRotator);
		Competitor->AreaID = AreaID;
	}

	for (int i = 0; i < VillageManagerSystem->StoreAreaID.Num(); i++) {
		int32 AreaID = VillageManagerSystem->StoreAreaID[i];
		AStore* Store = GetWorld()->SpawnActor<AStore>(BP_Store, *AreaLocMap.Find(AreaID), FRotator::ZeroRotator);
	}
}

void AVillageManager::RunDayTimer()
{
	GetWorld()->GetTimerManager().SetTimer(LeftDayTimeHandler, this, &AVillageManager::DecreaseDayTime, 1.0f, true);
}

void AVillageManager::BeginPlay()
{
	Super::BeginPlay();

	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();
	CustomerDataManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UCustomerDataManagerSystem>();
	CookYourWayGameState = Cast<ACookYourWayGameState>(UGameplayStatics::GetGameState(GetWorld()));

	Init();
	RunDayTimer();
}

void AVillageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int32 AVillageManager::GetLeftMinute()
{
	return LeftMinute;
}

int32 AVillageManager::GetLeftSecond()
{
	return LeftSecond;
}

void AVillageManager::DecreaseDayTime()
{
	if (LeftSecond == 0) {
		if (LeftMinute > 0) {
			LeftMinute--;
			LeftSecond = 59;
		}
		else {
			CookYourWayGameState->SaveCookYourWayData();
		}
	}
	else {
		LeftSecond --;
	}
}
