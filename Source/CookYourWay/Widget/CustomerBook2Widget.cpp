// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CustomerBook2Widget.h"
#include <Kismet/GameplayStatics.h>
#include "Actor/VillageManager.h"
#include <GameInstance/CookYourWayGameInstance.h>

void UCustomerBook2Widget::NativeConstruct()
{
	Super::NativeConstruct();

	VillageManagerSystem = UCookYourWayGameInstance::GetVillageManagerSystemStatic(this);
	Init();
}


TArray<FString> UCustomerBook2Widget::GetSpawnedStoreName(FString CustName)
{
	TArray<FString> SpawnedStoreNameArr;

	for (auto StoreData : VillageManagerSystem->StoreDataArr) {

		if (StoreData.StoreTableData.StoreCust1 == CustName ||
			StoreData.StoreTableData.StoreCust2 == CustName ||
			StoreData.StoreTableData.StoreCust3 == CustName) {
			if (SpawnedStoreNameArr.Contains(StoreData.StoreTableData.StoreName)) {
				continue;
			}
			else {
				SpawnedStoreNameArr.Add(StoreData.StoreTableData.StoreName);
			}
		}
	}
	return SpawnedStoreNameArr;
}
