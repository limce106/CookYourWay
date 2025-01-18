// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Table.h"
#include <Kismet/GameplayStatics.h>
#include "Reuben.h"

ATable::ATable()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ATable::BeginPlay()
{
	Super::BeginPlay();

	Reuben = Cast<AReuben>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void ATable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATable::PutActorOn(AActor* Actor)
{
	if (IsActorOn) {
		return;
	}

	Reuben->PutDownActor();

	FVector ActorLocation = GetActorLocation();
	ActorLocation.Z += 65.0f;
	Actor->SetActorLocation(ActorLocation);
	Actor->SetActorRotation(GetActorRotation());

	IsActorOn = true;
	PlacedActor = Actor;

	FRotator PlacedActorRotation = Reuben->GetActorRotation();
	PlacedActor->SetActorRotation(PlacedActorRotation);
}

void ATable::PickUpActor()
{
	if (!IsActorOn || Reuben->IsHold) {
		return;
	}

	Reuben->HoldActor(PlacedActor);

	IsActorOn = false;
	PlacedActor = NULL;
}

void ATable::TableInteraction()
{
	if (!Reuben->IsHold) {
		if (IsActorOn) {
			PickUpActor();
		}
	}
	// ���̺� ���� �ƹ��͵� ���ٸ� ����/������ġ �Ǵ� �������� �Ǵ� ��Ḧ ���̺� ���� �ø���.
	else {
		if (!IsActorOn) {
			PutActorOn(Reuben->HeldActor);
		}
	}
}

