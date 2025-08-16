// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Fridge.h"
#include <Widget/FridgeWidget.h>
#include <Kismet/GameplayStatics.h>
#include "Reuben.h"

AFridge::AFridge()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFridge::BeginPlay()
{
	Super::BeginPlay();
	
	Reuben = Cast<AReuben>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void AFridge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFridge::Interact_Implementation()
{
	UFridgeWidget* FridgeWidget = CreateWidget<UFridgeWidget>(GetWorld(), BP_FridgeWidget);
	FridgeWidget->AddToViewport();

	USoundBase* LoadedSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Assets/Sound/SoundAsset/SFX_FridgeOpen.SFX_FridgeOpen"));
	UGameplayStatics::PlaySoundAtLocation(this, LoadedSound, Reuben->GetActorLocation());
}

