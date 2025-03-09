// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/NewsEffectComponent.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/CustomerDataManagerSystem.h"
#include "GameInstance/VillageManagerSystem.h"
#include <GameInstance/CookYourWayGameInstance.h>

UNewsEffectComponent::UNewsEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UNewsEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	VillageManagerSystem = UCookYourWayGameInstance::GetVillageManagerSystemStatic(this);
	CustomerDataManagerSystem = UCookYourWayGameInstance::GetCustomerDataManagerSystemStatic(this);

	CustNameKorToEng();
	CurNewsEffect = VillageManagerSystem->NewsEffectCode;
	CurNewsKeyWord = VillageManagerSystem->NewsKeyWord;
}


void UNewsEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

TArray<float> UNewsEffectComponent::GetDecSpawnCustProb(TArray<FString> CustName)
{
	TArray<float> SpawnCustPercent;

	float NotDecreasedCustPercent = (1 - DecreasedSpawnCustProb) / (CustName.Num() - 1);

	for (int i = 0; i < CustName.Num(); i++) {
		if (CustName[i] == CurNewsKeyWord) {
			SpawnCustPercent.Add(DecreasedSpawnCustProb);
		}
		else {
			SpawnCustPercent.Add(NotDecreasedCustPercent);
		}
	}

	return SpawnCustPercent;
}

TArray<float> UNewsEffectComponent::GetIncSpawnCustProb(TArray<FString> CustName)
{
	TArray<float> SpawnCustPercent;

	float NotIncreasedCustPercent = (1 - IncreasedSpwanCustProb) / (CustName.Num() - 1);

	for (int i = 0; i < CustName.Num(); i++) {
		if (CustName[i] == CurNewsKeyWord) {
			SpawnCustPercent.Add(IncreasedSpwanCustProb);
		}
		else {
			SpawnCustPercent.Add(NotIncreasedCustPercent);
		}
	}

	return SpawnCustPercent;
}

void UNewsEffectComponent::CustNameKorToEng()
{
	// NewsEffectCode가 CustDec, CustInc라면 손님 이름이 한글로 저장되기 때문에 영어로 변환한다.
	if (VillageManagerSystem->NewsEffectCode == "CustDec" || VillageManagerSystem->NewsEffectCode == "CustInc") {
		FString CustKorName = VillageManagerSystem->NewsKeyWord;
		int32 index = 0;

		for (auto CustData : CustomerDataManagerSystem->CustomerTableRows) {
			if (CustKorName == CustData->CustName) {
				VillageManagerSystem->NewsKeyWord = CustomerDataManagerSystem->CustomerNames[index];
				break;
			}
			index++;
		}
	}
}
