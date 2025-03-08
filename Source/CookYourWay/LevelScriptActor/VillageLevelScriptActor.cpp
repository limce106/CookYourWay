// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelScriptActor/VillageLevelScriptActor.h"
#include <Kismet/GameplayStatics.h>
#include <Actor/CustomerPool.h>

AVillageLevelScriptActor::AVillageLevelScriptActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

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
	Super::BeginPlay();

	CustomerDataManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UCustomerDataManagerSystem>();
}

void AVillageLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
