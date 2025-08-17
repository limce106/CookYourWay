// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Dessert.h"
#include "Reuben.h"
#include <Kismet/GameplayStatics.h>

ADessert::ADessert()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADessert::BeginPlay()
{
	Super::BeginPlay();
	
	Reuben = Cast<AReuben>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void ADessert::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADessert::OnPutDown_Implementation(AActor* PlaceTarget)
{
	Reuben->PutDownActor();

	FVector Location;

	if (ATable* Table = Cast<ATable>(PlaceTarget))
	{
		Location = Table->GetActorLocation();
		Location.Z += TableZOffset;
		SetActorLocation(Location);
	}
	else if (ADiningTable* DiningTable = Cast<ADiningTable>(PlaceTarget))
	{
		Location = DiningTable->GetActorLocation();
		Location.Z += DiningTableZOffset;
		SetActorLocation(Location);
	}

	FRotator Rotation = PlaceTarget->GetActorRotation();
	SetActorRotation(Rotation);

	USoundBase* LoadedSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Assets/Sound/SoundAsset/SFX_PutIngredients.SFX_PutIngredients"));
	UGameplayStatics::PlaySoundAtLocation(this, LoadedSound, GetActorLocation());
}

void ADessert::OnPickUp_Implementation()
{
	Reuben->HoldActor(this);

	USoundBase* LoadedSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Assets/Sound/SoundAsset/SFX_Grab.SFX_Grab"));
	UGameplayStatics::PlaySoundAtLocation(this, LoadedSound, GetActorLocation());
}
