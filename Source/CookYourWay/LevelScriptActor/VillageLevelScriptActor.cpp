// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelScriptActor/VillageLevelScriptActor.h"
#include <Kismet/GameplayStatics.h>

void AVillageLevelScriptActor::Init()
{
	GetWorld()->SpawnActor<AActor>(BP_VillageManager, FVector::ZeroVector, FRotator::ZeroRotator);

	if (BP_PlayerBistroHUDClass) {
		UUserWidget* PlayerBistroHUD = CreateWidget<UUserWidget>(GetWorld(), BP_PlayerBistroHUDClass);
		PlayerBistroHUD->AddToViewport();
	}

	CustomerDataManagerSystem->AddCompetitorRegularCust();
}

void AVillageLevelScriptActor::BeginPlay()
{
	CustomerDataManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UCustomerDataManagerSystem>();

	Init();
}

void AVillageLevelScriptActor::Tick(float DeltaTime)
{

}
