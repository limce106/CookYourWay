// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/CustomerDataManagerSystem.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "VillageManagerSystem.h"
#include <Widget/FridgeIngrWidget.h>
#include <Actor/VillageManager.h>

UCustomerDataManagerSystem::UCustomerDataManagerSystem()
{
	FString CustomerTablePath = TEXT("/Game/Assets/Table/Customer.Customer");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CUSTOMERTABLE(*CustomerTablePath);
	CustomerTable = DT_CUSTOMERTABLE.Object;

	FString CustomerReviewTablePath = TEXT("/Game/Assets/Table/CustomerReview.CustomerReview");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CUSTOMERREVIEWTABLE(*CustomerReviewTablePath);
	CustomerReviewTable = DT_CUSTOMERREVIEWTABLE.Object;

	FString CustomerCommentTablePath = TEXT("/Game/Assets/Table/CustComment.CustComment");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CUSTOMERCOMMENTTABLE(*CustomerCommentTablePath);
	CustomerCommentTable = DT_CUSTOMERCOMMENTTABLE.Object;

	CustomerReviewTable->GetAllRows<FCustomerReviewData>("Get All Rows Of CustomerReviewData", CustomerReviewTableRows);
	CustomerCommentTable->GetAllRows<FCustomerCommentData>("Get All Rows Of CustomerCommentData", CustomerCommentTableRows);
	CustomerTable->GetAllRows<FCustomerData>("Get All Rows Of CustomerData", CustomerTableRows);

	for (int i = 0; i < CustomerTableRows.Num(); i++) {
		CustomerNames.Add(CustomerTable->GetRowNames()[i].ToString());
	}
}

void UCustomerDataManagerSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
}

void UCustomerDataManagerSystem::Init()
{
	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();
	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();

	if (VillageManagerSystem) {
		for (int CustNameIdx = 0; CustNameIdx < CustomerNames.Num(); CustNameIdx++) {
			FCustomerBistroKey CustomerBistroKey = GetCustomerBistroKey(CustomerNames[CustNameIdx], VillageManagerSystem->PlayerBistroAreaID);

			IsRegularCustMap.Add(CustomerBistroKey, false);
			LoyaltyMap.Add(CustomerBistroKey, 0);
			MaxSatisfactionMap.Add(CustomerBistroKey, 0.0f);
		}

		for (int CustNameIdx = 0; CustNameIdx < CustomerNames.Num(); CustNameIdx++) {
			for (int AreaIDIdx = 0; AreaIDIdx < VillageManagerSystem->CompetitorDataArr.Num(); AreaIDIdx++) {
				FCustomerBistroKey CustomerBistroKey = GetCustomerBistroKey(CustomerNames[CustNameIdx], VillageManagerSystem->CompetitorDataArr[AreaIDIdx].AreaID);

				IsRegularCustMap.Add(CustomerBistroKey, false);
				LoyaltyMap.Add(CustomerBistroKey, 0);
				MaxSatisfactionMap.Add(CustomerBistroKey, 0.0f);
			}
		}
	}
}

void UCustomerDataManagerSystem::RedefineCustomerComment() {
	AVillageManager* VillageManager = Cast<AVillageManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AVillageManager::StaticClass()));
	VillageManager->RedefinedCustomerCommentTableRows = CustomerCommentTableRows;

	for (int i = 0; i < CustomerCommentTableRows.Num(); i++) {
		if (CustomerCommentTableRows[i]->CustCommentType == 2) {
			FString RedefinedStr = RedefineTasteHintComment(CustomerCommentTableRows[i]->CustCode, CustomerCommentTableRows[i]->CustCommentString);
			VillageManager->RedefinedCustomerCommentTableRows[i]->CustCommentString = RedefinedStr;
		}
	}
}

FCustomerBistroKey UCustomerDataManagerSystem::GetCustomerBistroKey(FString CustomerName, int32 BistroAreaID)
{
	FCustomerBistroKey Key;
	Key.CustomerName = CustomerName;
	Key.BistroAreaID = BistroAreaID;

	return Key;
}

FString UCustomerDataManagerSystem::GetRandomCustName()
{
	int32 RandomIdx = UKismetMathLibrary::RandomIntegerInRange(0, CustomerNames.Num() - 1);
	FString RandomCustName = CustomerNames[RandomIdx];
	return RandomCustName;
}

TArray<int32> UCustomerDataManagerSystem::GetRandomTaste()
{
	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();
	TArray<int32> Taste;

	TArray<int32> CanAddFillings = IngredientManagerSystem->FillingIdxs;
	// 속재료는 항상 3개를 선택하도록 함
	for (int i = 0; i < 3; i++) {
		int FillingIndex = UKismetMathLibrary::RandomIntegerInRange(0, CanAddFillings.Num() - 1);
		Taste.Add(CanAddFillings[FillingIndex]);
		CanAddFillings.RemoveAt(FillingIndex);
	}

	int MeatIndex = UKismetMathLibrary::RandomIntegerInRange(0, IngredientManagerSystem->MeatIdxs.Num() - 1);
	Taste.Add(IngredientManagerSystem->MeatIdxs[MeatIndex]);

	int SauceIndex = UKismetMathLibrary::RandomIntegerInRange(0, IngredientManagerSystem->SauceIdxs.Num() - 1);
	Taste.Add(IngredientManagerSystem->SauceIdxs[SauceIndex]);

	//// 테스트
	//Taste.Add(12);
	//Taste.Add(12);
	////

	return Taste;
}

void UCustomerDataManagerSystem::SetRandomCustTastes()
{
	for (int i = 0; i < CustomerNames.Num(); i++) {
		CustNameToTasteMap.Add(CustomerNames[i], GetRandomTaste());
	}
}

void UCustomerDataManagerSystem::SetIngrSeasonCustTastes()
{
	FString SeasonIngr = VillageManagerSystem->NewsKeyWord;
	int32 SeasonIngrIdx;
	for (int i = 0; i < IngredientManagerSystem->IngredientRows.Num(); i++) {
		if (IngredientManagerSystem->IngredientRows[i].IngrName == SeasonIngr) {
			SeasonIngrIdx = i;
			break;
		}
	}

	for (int i = 0; i < CustomerNames.Num(); i++) {
		TArray<int32> Taste = GetRandomTaste();
		bool IsAlreadyHaveSeasonIngr = Taste.Contains(SeasonIngrIdx);
		if (!IsAlreadyHaveSeasonIngr) {
			Taste.Add(SeasonIngrIdx);

			// 시즌 재료가 속재료라면
			if (IngredientManagerSystem->IngredientRows[SeasonIngrIdx].IngrType == "Filling") {
				// 랜덤으로 취향 식재료 하나 제외
				int32 TasteArrIdx = UKismetMathLibrary::RandomIntegerInRange(0, 2);
				Taste.RemoveAt(TasteArrIdx);
			}
			else if (IngredientManagerSystem->IngredientRows[SeasonIngrIdx].IngrType == "Meat") {
				Taste.RemoveAt(3);
			}
			else if (IngredientManagerSystem->IngredientRows[SeasonIngrIdx].IngrType == "Sauce") {
				Taste.RemoveAt(4);
			}
		}

		CustNameToTasteMap.Add(CustomerNames[i], Taste);
	}
}

void UCustomerDataManagerSystem::SetPopularIngrCustTastes()
{
	FString PopularIngr = VillageManagerSystem->NewsKeyWord;
	int32 PopularIngrIdx;
	for (int i = 0; i < IngredientManagerSystem->IngredientRows.Num(); i++) {
		if (IngredientManagerSystem->IngredientRows[i].IngrName == PopularIngr) {
			PopularIngrIdx = i;
			break;
		}
	}

	for (int i = 0; i < CustomerNames.Num(); i++) {
		TArray<int32> Taste = GetRandomTaste();

		float Probability = FMath::FRand();
		if (Probability > 0.5f) {
			bool IsAlreadyHavePopularIngr = Taste.Contains(PopularIngrIdx);
			if (!IsAlreadyHavePopularIngr) {
				Taste.Add(PopularIngrIdx);

				if (IngredientManagerSystem->IngredientRows[PopularIngrIdx].IngrType == "Filling") {
					int32 TasteArrIdx = UKismetMathLibrary::RandomIntegerInRange(0, 2);
					Taste.RemoveAt(TasteArrIdx);
				}
				else if (IngredientManagerSystem->IngredientRows[PopularIngrIdx].IngrType == "Meat") {
					Taste.RemoveAt(3);
				}
				else if (IngredientManagerSystem->IngredientRows[PopularIngrIdx].IngrType == "Sauce") {
					Taste.RemoveAt(4);
				}
			}
		}

		CustNameToTasteMap.Add(CustomerNames[i], Taste);
	}
}

void UCustomerDataManagerSystem::SetCustTastes()
{
	CustNameToTasteMap.Empty();

	if (VillageManagerSystem->NewsEffectCode != "" && VillageManagerSystem->NewsEffectCode.Contains("IngrSeasonDay")) {
		SetIngrSeasonCustTastes();
	}
	else if (VillageManagerSystem->NewsEffectCode != "" && VillageManagerSystem->NewsEffectCode.Contains("PopularTaste")) {
		SetPopularIngrCustTastes();
	}
	else {
		SetRandomCustTastes();
	}

	RedefineCustomerComment();

	// 테스트
	for (int i = 0; i < CustomerNames.Num(); i++) {
		TArray<int32> Taste = CustNameToTasteMap[CustomerNames[i]];
		for (int j = 0; j < Taste.Num(); j++) {
			UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *CustomerNames[i], *IngredientManagerSystem->IngredientRows[Taste[j]].IngrName);
		}
	}
	//
}

TArray<int32> UCustomerDataManagerSystem::GetCustTaste(FString CustName)
{
	TArray<int32> Taste = *CustNameToTasteMap.Find(CustName);
	return Taste;
}

bool UCustomerDataManagerSystem::IsRegularCust(FString CustomerName, int32 BistroAreaID)
{
	FCustomerBistroKey Key = GetCustomerBistroKey(CustomerName, BistroAreaID);
	bool IsRegular = false;
	if (IsRegularCustMap.Find(Key))
	{
		IsRegular = true;
	}

	return IsRegular;
}

void UCustomerDataManagerSystem::AddRegularCust(FString CustomerName, int32 BistroAreaID)
{
	FCustomerBistroKey Key = GetCustomerBistroKey(CustomerName, BistroAreaID);
	IsRegularCustMap.Add(Key, true);
	LoyaltyMap.Add(Key, 100);
}

void UCustomerDataManagerSystem::DecreaseLoyalty(FString CustomerName, int32 BistroAreaID, float Decreasement)
{
	FCustomerBistroKey Key = GetCustomerBistroKey(CustomerName, BistroAreaID);
	int32 CurLoyalty = *LoyaltyMap.Find(Key);

	CurLoyalty -= Decreasement;
	if (CurLoyalty <= 0) {
		CurLoyalty = 0;
		// 단골손님에서 제외
		IsRegularCustMap.Add(Key, false);
		MaxSatisfactionMap.Add(Key, 0);
	}
	LoyaltyMap.Add(Key, CurLoyalty);
}

void UCustomerDataManagerSystem::DecreaseCompetitorLoyalty(int32 CompetitorAreaID)
{
	for (auto CustomerName : CustomerNames) {
		// 단골 손님인 경우에만 계산 진행
		FCustomerBistroKey Key = GetCustomerBistroKey(CustomerName, CompetitorAreaID);
		if (LoyaltyMap.Find(Key) == false) {
			continue;
		}

		float Decreasement = 0;
		int32 Rand = UKismetMathLibrary::RandomIntegerInRange(1, 100);

		if (Rand <= 10) {
			Decreasement = 0;
		}
		else if (Rand <= 40) {
			Decreasement = 5;
		}
		else if (Rand <= 80) {
			Decreasement = 10;
		}
		else if (Rand <= 95) {
			Decreasement = 15;
		}
		else {
			Decreasement = 20;
		}

		DecreaseLoyalty(CustomerName, CompetitorAreaID, Decreasement);
	}
}

void UCustomerDataManagerSystem::AddCompetitorRegularCust()
{
	for (auto CompetitorData : VillageManagerSystem->CompetitorDataArr) {
		int32 Rand = UKismetMathLibrary::RandomIntegerInRange(1, 100);
		if (Rand <= 5) {
			int32 RandomCust = UKismetMathLibrary::RandomIntegerInRange(0, CustomerNames.Num() - 1);
			AddRegularCust(CustomerNames[RandomCust], CompetitorData.AreaID);
		}
	}
}

void UCustomerDataManagerSystem::UpdateMaxSatisfaction(FString CustName, int32 BistroAreaID, int32 Satisfaction)
{
	FCustomerBistroKey Key = GetCustomerBistroKey(CustName, BistroAreaID);
	int32 CurMaxSatisfaction = *MaxSatisfactionMap.Find(Key);

	if (Satisfaction > CurMaxSatisfaction) {
		MaxSatisfactionMap.Add(Key, Satisfaction);
	}
}

bool UCustomerDataManagerSystem::HasRegularCust(int32 BistroAreaID)
{
	for (auto CustName : CustomerNames) {
		FCustomerBistroKey Key = GetCustomerBistroKey(CustName, BistroAreaID);
		bool IsRegualrCust = *IsRegularCustMap.Find(Key);

		if (IsRegualrCust) {
			return true;
		}
	}

	return false;
}

FString UCustomerDataManagerSystem::GetCustReviewDialogue(FString CustName, int32 TasteScore)
{
	for (auto CustReviewData : CustomerReviewTableRows) {
		if (CustReviewData->CustCode == CustName && CustReviewData->ReviewRating == TasteScore) {
			return CustReviewData->ReviewString;
		}
	}
	return FString();
}

TArray<int32> UCustomerDataManagerSystem::GetCustNameToTasteMapValue(FString CustName)
{
	if (CustNameToTasteMap.Find(CustName)) {
		return CustNameToTasteMap[CustName];
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Can't Get CustNameToTasteMap Value!"));
		return TArray<int32>();
	}
}

bool UCustomerDataManagerSystem::GetIsRegularCustMapValue(FCustomerBistroKey Key)
{
	if (IsRegularCustMap.Find(Key)) {
		return IsRegularCustMap[Key];
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Can't Get IsRegularCustMap Value!"));
		return false;
	}
}

int32 UCustomerDataManagerSystem::GetLoyaltyMapValue(FCustomerBistroKey Key)
{
	if (LoyaltyMap.Find(Key)) {
		return LoyaltyMap[Key];
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Can't Get LoyaltyMap Value!"));
		return 0;
	}
}

int32 UCustomerDataManagerSystem::GetMaxSatisfactionMapValue(FCustomerBistroKey Key)
{
	if (MaxSatisfactionMap.Find(Key)) {
		return MaxSatisfactionMap[Key];
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Can't Get MaxSatisfactionMap Value!"));
		return 0;
	}
}

FString UCustomerDataManagerSystem::RedefineTasteHintComment(FString CustName, FString Comment)
{
	FString Redefined = Comment;
	FString tmp1;
	FString tmp2;

	TArray<int32> Tastes = CustNameToTasteMap[CustName];
	int32 OneTasteIdx = UKismetMathLibrary::RandomIntegerInRange(0, Tastes.Num() - 1);
	FString OneTasteKor = IngredientManagerSystem->IngredientRows[Tastes[OneTasteIdx]].IngrName;

	for (int idx = 0; idx < Redefined.Len(); idx++) {
		if (Redefined[idx] == '{') {
			tmp1 = Redefined.Mid(0, idx);
			tmp2 = Redefined.Mid(idx + 3, Redefined.Len() - (idx + 3));

			Redefined = (tmp1.Append(OneTasteKor)).Append(tmp2);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Original %s: %s"), *CustName, *Comment);
	UE_LOG(LogTemp, Warning, TEXT("Comment %s: %s"), *CustName, *OneTasteKor);
	UE_LOG(LogTemp, Warning, TEXT("Redefined %s: %s"), *CustName, *Redefined);

	return Redefined;
}

TArray<FPlayerBistroRatingData> UCustomerDataManagerSystem::GetGreaterSortedPlayerBistroRating()
{
	TArray<FPlayerBistroRatingData> CopyRatingArr = PlayerBistroRatingDataArr;

	CopyRatingArr.Sort([](const FPlayerBistroRatingData& A, const FPlayerBistroRatingData& B)
		{
			return A.Rating > B.Rating;
		});
	return CopyRatingArr;
}
