// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CustomerBook2Widget.h"
#include <Kismet/GameplayStatics.h>
#include "Actor/VillageManager.h"

void UCustomerBook2Widget::NativeConstruct()
{
	Super::NativeConstruct();

	VillageManager = Cast<AVillageManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AVillageManager::StaticClass()));
	Init();
}


TArray<FString> UCustomerBook2Widget::GetSpawnedStoreName(FString CustName)
{
	TArray<FString> SpawnedStoreNameArr;

	for (auto StoreData : VillageManager->VillageManagerSystem->StoreDataArr) {

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
