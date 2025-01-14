// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelScriptActor/VillageLevelScriptActor.h"
#include <Kismet/GameplayStatics.h>

void AVillageLevelScriptActor::Init()
{
	if (BP_PlayerBistroHUDClass) {
		UUserWidget* PlayerBistroHUD = CreateWidget<UUserWidget>(GetWorld(), BP_PlayerBistroHUDClass);
		PlayerBistroHUD->AddToViewport();
	}

	CustomerDataManagerSystem->SetAllCustTastes();
	CustomerDataManagerSystem->AddCompetitorRegularCust();
}

void AVillageLevelScriptActor::BeginPlay()
{
	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();
	CustomerDataManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UCustomerDataManagerSystem>();

	Init();
}

void AVillageLevelScriptActor::Tick(float DeltaTime)
{

}
