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
	SpawnBistrosAndStore();
	VillageManagerSystem->ElapseCompetitorOpenPromoDay();

	// 테스트
	CustomerDataManagerSystem->SetCustTastes();
	//
	CustomerDataManagerSystem->RedefineCustomerComment();
	IsCommentTalked.Init(false, RedefinedCustomerCommentTableRows.Num());
}

void AVillageManager::RunDayTimer()
{
	GetWorld()->GetTimerManager().SetTimer(LeftDayTimeHandler, this, &AVillageManager::DecreaseDayTime, 1.0f, true);
}

void AVillageManager::SpawnBistrosAndStore()
{
	APlayerBistro* SpawnedPlayerBistro = GetWorld()->SpawnActor<APlayerBistro>(BP_PlayerBistro, *AreaLocMap.Find(VillageManagerSystem->PlayerBistroAreaID), FRotator::ZeroRotator);
	SpawnedPlayerBistro->AreaID = VillageManagerSystem->PlayerBistroAreaID;

	for (int i = 0; i < VillageManagerSystem->CompetitorDataArr.Num(); i++) {
		int32 AreaID = VillageManagerSystem->CompetitorDataArr[i].AreaID;
		ACompetitor* Competitor = GetWorld()->SpawnActor<ACompetitor>(BP_Competitor, *AreaLocMap.Find(AreaID), FRotator::ZeroRotator);
		Competitor->AreaID = AreaID;

		if (VillageManagerSystem->CompetitorDataArr[i].OpenPromoDay > 0) {
			Competitor->IsOpenPromo = true;
		}
		else {
			Competitor->IsOpenPromo = false;
		}
	}

	CustomerPool->InitCustomerPool();

	for (int i = 0; i < VillageManagerSystem->StoreDataArr.Num(); i++) {
		FStoreTable StoreTableData = VillageManagerSystem->StoreDataArr[i].StoreTableData;
		AStore* Store = GetWorld()->SpawnActor<AStore>(BP_Store, *AreaLocMap.Find(VillageManagerSystem->StoreDataArr[i].AreaID), FRotator::ZeroRotator);
		Store->InitializeStoreTableData(VillageManagerSystem->StoreDataArr[i].AreaID, StoreTableData);
		Store->Init();
	}
}

void AVillageManager::DecreaseStorePeriod()
{
	if (VillageManagerSystem->StoreDataArr.Num() == 0) {
		return;
	}

	for (int i = 0; i < VillageManagerSystem->StoreDataArr.Num(); i++) {
		VillageManagerSystem->StoreDataArr[i].StoreTableData.StorePeriod--;
	}
}

void AVillageManager::BeginPlay()
{
	Super::BeginPlay();

	CustomerDataManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UCustomerDataManagerSystem>();
	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();
	CookYourWayGameState = Cast<ACookYourWayGameState>(UGameplayStatics::GetGameState(GetWorld()));
	CustomerPool = Cast<ACustomerPool>(UGameplayStatics::GetActorOfClass(GetWorld(), BP_CustomerPool));

	Init();
	RunDayTimer();

	PlayerBistro = Cast<APlayerBistro>(UGameplayStatics::GetActorOfClass(GetWorld(), BP_PlayerBistro));
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
	for (int i = VillageManagerSystem->CompetitorDataArr.Num() - 1; i >= 0; i--) {
		FCompetitorData ComptData = VillageManagerSystem->CompetitorDataArr[i];

		// 단골 손님 보유 여부
		bool HasRegularCust = CustomerDataManagerSystem->HasRegularCust(ComptData.AreaID);

		if (!HasRegularCust) {
			VillageManagerSystem->CompetitorDataArr.Remove(ComptData);
			VillageManagerSystem->DestroyedCompetitor++;

			int32 NewCompetitorAreaID = GetRandomAreaId();
			FCompetitorData* NewCompetitorData = new FCompetitorData(NewCompetitorAreaID, 3.5);
			NewCompetitorData->ComptName = VillageManagerSystem->GetRandomComptName();
			VillageManagerSystem->CompetitorDataArr.Add(*NewCompetitorData);
			
			for (auto CustName : CustomerDataManagerSystem->CustomerNames) {
				FCustomerBistroKey CurKey = CustomerDataManagerSystem->GetCustomerBistroKey(CustName, ComptData.AreaID);
				FCustomerBistroKey NewKey = CustomerDataManagerSystem->GetCustomerBistroKey(CustName, NewCompetitorAreaID);

				CustomerDataManagerSystem->IsRegularCustMap.Remove(CurKey);
				CustomerDataManagerSystem->LoyaltyMap.Remove(CurKey);
				CustomerDataManagerSystem->MaxSatisfactionMap.Remove(CurKey);

				CustomerDataManagerSystem->IsRegularCustMap.Add(NewKey, false);
				CustomerDataManagerSystem->LoyaltyMap.Add(NewKey, 0);
				CustomerDataManagerSystem->MaxSatisfactionMap.Add(NewKey, 0.0f);
			}

			// 오픈 프로모션: 전체 손님 유형 중 랜덤으로 2개의 유형을 초기 단골 손님으로 만든다.
			AddRandomRegularCust(NewCompetitorAreaID, 2);

			if (VillageManagerSystem->DestroyedCompetitor <= 8) {
				CreateNewStore();
			}
		}
	}
}

void AVillageManager::TryCreateNewStore()
{
	for (int i = VillageManagerSystem->StoreDataArr.Num() - 1; i >= 0; i--) {
		if (VillageManagerSystem->StoreDataArr[i].StoreTableData.StorePeriod == 0) {
			VillageManagerSystem->StoreDataArr.RemoveAt(i);
			CreateNewStore();
		}
	}
}

void AVillageManager::CreateNewStore()
{
	int32 NewAreaID = GetRandomAreaId();
	int32 RandomStoreIdx = UKismetMathLibrary::RandomIntegerInRange(0, VillageManagerSystem->StoreTableRows.Num() - 1);
	FStoreTable* NewStoreTableData = VillageManagerSystem->StoreTableRows[RandomStoreIdx];
	FStoreData NewStoreData(NewAreaID, NewStoreTableData);
	VillageManagerSystem->StoreDataArr.Add(NewStoreData);
}

void AVillageManager::AddRandomRegularCust(int32 AreaID, int32 RegularCustNum)
{
	for (int i = 0; i < RegularCustNum; i++) {
		int32 RegularCustNameIdx = UKismetMathLibrary::RandomIntegerInRange(0, CustomerDataManagerSystem->CustomerNames.Num() - 1);
		FString RegularCustName = CustomerDataManagerSystem->CustomerNames[RegularCustNameIdx];

		CustomerDataManagerSystem->AddRegularCust(RegularCustName, AreaID);
	}
}

int32 AVillageManager::GetRandomAreaId()
{
	TArray<int32> CompetitorAreaID;
	for (FCompetitorData CompetitorData : VillageManagerSystem->CompetitorDataArr) {
		CompetitorAreaID.Add(CompetitorData.AreaID);
	}

	TArray<int32> StoreAreaID;
	for (FStoreData StoreData : VillageManagerSystem->StoreDataArr) {
		StoreAreaID.Add(StoreData.AreaID);
	}

	TArray<int32> BlankAreaID;
	// 기존 경쟁사, 상점과 플레이어 가게와 중복되지 않는 부지
	for (int i = 1; i <= AreaLocMap.Num(); i++) {
		if (i != VillageManagerSystem->PlayerBistroAreaID || !CompetitorAreaID.Contains(i) || !StoreAreaID.Contains(i)) {
			BlankAreaID.Add(i);
		}
	}

	int32 RandomAreaId = UKismetMathLibrary::RandomIntegerInRange(0, BlankAreaID.Num() - 1);
	return 	BlankAreaID[RandomAreaId];
}

void AVillageManager::EndEatingCustomer()
{
	// 플레이어 가게에서 아직 나가지 않고 음식을 먹던 손님도 평점, 방문한 손님 수에 포함
	TArray<AActor*> AllDiningTableActorArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerBistro->BP_DiningTable, AllDiningTableActorArr);
	for (auto Actor : AllDiningTableActorArr) {
		ADiningTable* DiningTable = Cast<ADiningTable>(Actor);

		if (DiningTable->SeatedCustomer && DiningTable->SeatedCustomer->IsEat) {
			PlayerBistro->LeaveCustomerInBistro(DiningTable->SeatedCustomer);
		}
	}
}

void AVillageManager::SaveStoreDataInManager()
{
	VillageManagerSystem->StoreDataArr.Empty();
	TArray<AActor*> AllStoreActorArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BP_Store, AllStoreActorArr);
	for (auto Actor : AllStoreActorArr) {
		AStore* Store = Cast<AStore>(Actor);
		FStoreData StoreData(Store->AreaID, &Store->CurStoreTableData);
		VillageManagerSystem->StoreDataArr.Add(StoreData);
	}

	for (auto CompetitorData : VillageManagerSystem->CompetitorDataArr) {
		CompetitorData.IsComptFestival = false;
	}
}

void AVillageManager::EndDay()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	EndEatingCustomer();
	SaveStoreDataInManager();
	DecreaseStorePeriod();

	// 일요일 저녁이라면
	if (VillageManagerSystem->IsSunday()) {
		CustomerDataManagerSystem->PlayerBistroRatingDataArr.Empty();
		VillageManagerSystem->InitCompetitorRatingDataArr();
		TryCreateNewCompetitor();
	}
	TryCreateNewStore();

	CookYourWayGameState->SaveCookYourWayData();

	StartFadeOutAnim();
	StartSubtractAnim();
}

FString AVillageManager::DayToWeekString(int32 Day)
{
	FString WeekString;
	// 주차
	int32 Week = (Day - 1) / 7 + 1;
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

void AVillageManager::UpdateProfitsValue(int32 Value)
{
	VillageManagerSystem->TotalAsset += Value;
	PlayerBistro->TodayNetIncome += Value;

	if (Value > 0) {
		PlayerBistro->TodaySoldPrice += Value;
	}
}