// Fill out your copyright notice in the Description page of Project Settings.


#include "TableManagerSystem.h"

UTableManagerSystem::UTableManagerSystem() {
	FString PlayerBistroTablePath = TEXT("/Game/PastaYourWay_Assets/Table/PlayerBistro.PlayerBistro");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_PLAYERBISTROTABLE(*PlayerBistroTablePath);
	PlayerBistroTable = DT_PLAYERBISTROTABLE.Object;

	FString CustomerTablePath = TEXT("/Game/PastaYourWay_Assets/Table/Customer.Customer");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CUSTOMERTABLE(*CustomerTablePath);
	CustomerTable = DT_CUSTOMERTABLE.Object;

	PlayerBistroTable->GetAllRows<FPlayerBistroData>("GetAllRows", PlayerBistroTableRows);
	CustomerTable->GetAllRows<FCustomerData>("GetAllRows", CustomerTableRows);
}