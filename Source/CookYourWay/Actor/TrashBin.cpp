// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TrashBin.h"
#include "Reuben.h"
#include <Kismet/GameplayStatics.h>

ATrashBin::ATrashBin()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ATrashBin::BeginPlay()
{
	Super::BeginPlay();
	
	Reuben = Cast<AReuben>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void ATrashBin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrashBin::Interact_Implementation()
{
	if (!Reuben->IsHold) {
		return;
	}

	if (ASandwich* Sandwich = Cast<ASandwich>(Reuben->HeldActor)) {
		Sandwich->DestroySandwich();

		USoundBase* LoadedSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Assets/Sound/SoundAsset/SFX_Trash.SFX_Trash"));
		UGameplayStatics::PlaySound2D(this, LoadedSound);
	}
	else {
		Reuben->HeldActor->Destroy();

		USoundBase* LoadedSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Assets/Sound/SoundAsset/SFX_Trash.SFX_Trash"));
		UGameplayStatics::PlaySound2D(this, LoadedSound);
	}

	Reuben->IsHold = false;
	Reuben->HeldActor = nullptr;
}

