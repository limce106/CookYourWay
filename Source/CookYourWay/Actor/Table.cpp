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
	// 테이블 위에 아무것도 없다면 접시/샌드위치 또는 조리도구 또는 재료를 테이블 위로 올린다.
	else {
		if (!IsActorOn) {
			PutActorOn(Reuben->HeldActor);
		}
	}
}

