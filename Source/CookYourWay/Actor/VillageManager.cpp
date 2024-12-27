// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/VillageManager.h"
#include "PlayerBistro.h"
#include "Competitor.h"
#include "Store.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "Blueprint/UserWidget.h"

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
	// 월요일이면
	if (VillageManagerSystem->Day != 1 && VillageManagerSystem->Day % 7 == 1) {
		TryCreateNewCompetitor();
	}

	SpawnBistrosAndStore();
}

void AVillageManager::RunDayTimer()
{
	GetWorld()->GetTimerManager().SetTimer(LeftDayTimeHandler, this, &AVillageManager::DecreaseDayTime, 1.0f, true);
}

void AVillageManager::SpawnBistrosAndStore()
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
			GetWorld()->GetTimerManager().PauseTimer(LeftDayTimeHandler);
			EndDay();
		}
	}
	else {
		LeftSecond --;
	}
}
void AVillageManager::TryCreateNewCompetitor()
{
	for (auto CurCompetitorAreaID : VillageManagerSystem->CompetitorAreaID) {
		// 단골 손님 보유 여부
		bool HasRegularCust = CustomerDataManagerSystem->HasRegularCust(CurCompetitorAreaID);

		if (!HasRegularCust) {
			int32 NewCompetitorAreaID = GetRandomAreaId();
			VillageManagerSystem->CompetitorAreaID.Remove(CurCompetitorAreaID);
			VillageManagerSystem->CompetitorAreaID.Add(NewCompetitorAreaID);
			
			for (auto CustName : CustomerDataManagerSystem->CustomerNames) {
				FCustomerBistroKey CurKey = CustomerDataManagerSystem->GetCustomerBistroKey(CustName, CurCompetitorAreaID);
				FCustomerBistroKey NewKey = CustomerDataManagerSystem->GetCustomerBistroKey(CustName, NewCompetitorAreaID);

				CustomerDataManagerSystem->IsRegularCustMap.Remove(CurKey);
				CustomerDataManagerSystem->LoyaltyMap.Remove(CurKey);
				CustomerDataManagerSystem->AvgRateMap.Remove(CurKey);
				VillageManagerSystem->CompetitorTotalCust.Remove(CurCompetitorAreaID);

				CustomerDataManagerSystem->IsRegularCustMap.Add(NewKey, false);
				CustomerDataManagerSystem->LoyaltyMap.Add(NewKey, 0.0f);
				CustomerDataManagerSystem->AvgRateMap.Add(NewKey, 0.0f);
				VillageManagerSystem->CompetitorTotalCust.Add(NewCompetitorAreaID, 0);
			}
		}
	}
}

int32 AVillageManager::GetRandomAreaId()
{
	// 기존 경쟁사, 상점과 플레이어 가게와 중복되지 않는 부지
	int32 RandomAreaId = UKismetMathLibrary::RandomIntegerInRange(1, AreaLocMap.Num());

	if (RandomAreaId == VillageManagerSystem->PlayerBistroAreaID ||
		VillageManagerSystem->CompetitorAreaID.Contains(RandomAreaId) ||
		VillageManagerSystem->StoreAreaID.Contains(RandomAreaId)) {
		GetRandomAreaId();
		return 0;
	}
	else {
		return RandomAreaId;
	}
}

void AVillageManager::EndDay()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	CookYourWayGameState->SaveCookYourWayData();

	UUserWidget* BP_Subtract = CreateWidget<UUserWidget>(GetWorld(), BP_SubtractClass);
	if (BP_Subtract) {
		BP_Subtract->AddToViewport();
	}
}

FString AVillageManager::DayToWeekString(int32 Day)
{
	FString WeekString;
	// 주차
	int32 Week = Day / 7 + 1;
	WeekString = FString::Printf(TEXT("%d주차 "), Week);

	// 요일
	int32 Remainder = Day % 7;
	FString DayStr;
	if (Remainder == 1) {
		DayStr = FString(TEXT("월요일"));
	}
	else if (Remainder == 2) {
		DayStr = FString(TEXT("화요일"));
	}
	else if (Remainder == 3) {
		DayStr = FString(TEXT("수요일"));
	}
	else if (Remainder == 4) {
		DayStr = FString(TEXT("목요일"));
	}
	else if (Remainder == 5) {
		DayStr = FString(TEXT("금요일"));
	}
	else if (Remainder == 6) {
		DayStr = FString(TEXT("토요일"));
	}
	else {
		DayStr = FString(TEXT("일요일"));
	}

	WeekString.Append(DayStr);

	return WeekString;
}
