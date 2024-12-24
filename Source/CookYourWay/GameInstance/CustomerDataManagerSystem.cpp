// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/CustomerDataManagerSystem.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>

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
			LoyaltyMap.Add(CustomerBistroKey, 0.0f);
			AvgRateMap.Add(CustomerBistroKey, 0.0f);
		}

		for (int CustNameIdx = 0; CustNameIdx < CustomerNames.Num(); CustNameIdx++) {
			for (int AreaIDIdx = 0; AreaIDIdx < VillageManagerSystem->CompetitorAreaID.Num(); AreaIDIdx++) {
				FCustomerBistroKey CustomerBistroKey = GetCustomerBistroKey(CustomerNames[CustNameIdx], VillageManagerSystem->CompetitorAreaID[AreaIDIdx]);

				IsRegularCustMap.Add(CustomerBistroKey, false);
				LoyaltyMap.Add(CustomerBistroKey, 0.0f);
				AvgRateMap.Add(CustomerBistroKey, 0.0f);
			}
		}
	}

	SetAllCustTastes();
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

void UCustomerDataManagerSystem::SetAllCustTastes()
{
	for (int i = 0; i < CustomerNames.Num(); i++) {
		CustNameToTasteMap.Add(CustomerNames[i], GetRandomTaste());
	}
}

TArray<int32> UCustomerDataManagerSystem::GetCustTaste(FString CustName)
{
	TArray<int32> Taste = *CustNameToTasteMap.Find(CustName);
	return Taste;
}

float UCustomerDataManagerSystem::GetAvgRate(FString CustomerName, int32 BistroAreaID)
{
	FCustomerBistroKey Key = GetCustomerBistroKey(CustomerName, BistroAreaID);

	if (AvgRateMap.Contains(Key))
	{
		float* FoundAvgRate = AvgRateMap.Find(Key);
		return *FoundAvgRate;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Key not found: %s, %d"), *CustomerName, BistroAreaID);
		return 0;
	}
}

float UCustomerDataManagerSystem::GetLoyalty(FString CustomerName, int32 BistroAreaID)
{
	FCustomerBistroKey Key = GetCustomerBistroKey(CustomerName, BistroAreaID);
	float Loyalty = *LoyaltyMap.Find(Key);

	return Loyalty;
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

void UCustomerDataManagerSystem::UpdateAvgRate(FString CustomerName, int32 BistroAreaID, int32 VisitedCustNum, int32 ReveiwRate)
{
	FCustomerBistroKey Key = GetCustomerBistroKey(CustomerName, BistroAreaID);
	float TotalAvgRate = GetAvgRate(CustomerName, BistroAreaID);

	float UpdatedReviewAvg = (TotalAvgRate * (VisitedCustNum - 1) + ReveiwRate) / VisitedCustNum;

	AvgRateMap.Emplace(Key, UpdatedReviewAvg);
}
