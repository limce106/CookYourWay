// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/NewsEffectComponent.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/CustomerDataManagerSystem.h"
#include "GameInstance/VillageManagerSystem.h"

UNewsEffectComponent::UNewsEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UNewsEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	CustomerDataManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UCustomerDataManagerSystem>();
	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();

	CurNewsEffect = VillageManagerSystem->NewsEffectCode;
	CurNewsKeyWord = VillageManagerSystem->NewsKeyWord;
}


void UNewsEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

TArray<float> UNewsEffectComponent::GetDecSpawnCustPercent(TArray<FString> CustName)
{
	TArray<float> SpawnCustPercent;

	float NotDecreasedCustPercent = (1 - DecreasedSpawnCustPercent) / (CustName.Num() - 1);

	for (int i = 0; i < CustName.Num(); i++) {
		if (CustName[i] == CurNewsKeyWord) {
			SpawnCustPercent.Add(DecreasedSpawnCustPercent);
		}
		else {
			SpawnCustPercent.Add(NotDecreasedCustPercent);
		}
	}

	return SpawnCustPercent;
}

TArray<float> UNewsEffectComponent::GetIncSpawnCustPercent(TArray<FString> CustName)
{
	TArray<float> SpawnCustPercent;

	float NotIncreasedCustPercent = (1 - IncreasedSpwanCustPercent) / (CustName.Num() - 1);

	for (int i = 0; i < CustName.Num(); i++) {
		if (CustName[i] == CurNewsKeyWord) {
			SpawnCustPercent.Add(IncreasedSpwanCustPercent);
		}
		else {
			SpawnCustPercent.Add(NotIncreasedCustPercent);
		}
	}

	return SpawnCustPercent;
}

