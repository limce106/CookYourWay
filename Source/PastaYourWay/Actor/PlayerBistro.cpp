// Fill out your copyright notice in the Description page of Project Settings.


#include "PastaYourWay/Actor/PlayerBistro.h"
#include <Kismet/GameplayStatics.h>

APlayerBistro::APlayerBistro()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APlayerBistro::BeginPlay()
{
	Super::BeginPlay();
	
	TableManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UTableManagerSystem>();

	Level = TableManagerSystem->PlayerBistroTableRows[0]->PlayerLevel;
	Assets = TableManagerSystem->PlayerBistroTableRows[0]->PlayerAssets;
	Rating = TableManagerSystem->PlayerBistroTableRows[0]->PlayerBistroRating;
}

void APlayerBistro::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

