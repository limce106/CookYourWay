// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PartTimer.h"
#include <Components/ShapeComponent.h>
#include "Sandwich.h"
#include "Customer.h"
#include "DiningTable.h"
#include "Reuben.h"
#include "VillageManager.h"
#include <Kismet/GameplayStatics.h>

APartTimer::APartTimer()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APartTimer::BeginPlay()
{
	Super::BeginPlay();
	
}

void APartTimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHeldActorLoc();
}

void APartTimer::HoldActor(AActor* Actor)
{
	UShapeComponent* ActorCollision = Cast<UShapeComponent>(Actor->FindComponentByClass(UShapeComponent::StaticClass()));
	if (ActorCollision) {
		ActorCollision->SetCollisionProfileName(TEXT("NoCollision"));
	}

	HeldActor = Actor;

	FRotator HeldActorRotation = GetActorRotation();
	HeldActorRotation.Yaw -= 180.0f;
	HeldActor->SetActorRotation(HeldActorRotation);
}

void APartTimer::PutDownActor()
{
	UShapeComponent* ActorCollision = Cast<UShapeComponent>(HeldActor->FindComponentByClass(UShapeComponent::StaticClass()));
	if (ActorCollision) {
		ActorCollision->SetCollisionProfileName(TEXT("OnSomething"));
	}

	HeldActor = nullptr;
}

void APartTimer::SetHeldActorLoc()
{
	if (HeldActor) {
		FVector LeftHandLoc = GetMesh()->GetSocketLocation(FName("LeftHandSocket"));
		FVector RightHandLoc = GetMesh()->GetSocketLocation(FName("RightHandSocket"));

		FVector MidHandLoc = (LeftHandLoc + RightHandLoc) / 2;
		MidHandLoc.Z += 15.0f;

		HeldActor->SetActorLocation(MidHandLoc);
	}
}

TArray<int32> APartTimer::GetHoldingSandwichIngr()
{
	if (HeldActor && HeldActor->GetClass()->IsChildOf(ASandwich::StaticClass())) {
		ASandwich* Sandwich = Cast<ASandwich>(HeldActor);
		TArray<int32> Ingr = Sandwich->IngrActorToNum();
		if (Sandwich->IsFirstIngrBread()) {
			Ingr.RemoveAt(0);
		}
		if (Sandwich->IsLastIngrBread()) {
			Ingr.RemoveAt(Ingr.Num() - 1);
		}
		return Ingr;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Can't GetHoldingSandwichIngr!"));
		return TArray<int32>();
	}
}

void APartTimer::GiveSandwich()
{
	if (!TargetDiningTable) {
		UE_LOG(LogTemp, Error, TEXT("Invalid TargetDiningTable!"));
	}

	ACustomer* SeatedCustomer = TargetDiningTable->SeatedCustomer;
	ASandwich* Sandwich = Cast<ASandwich>(HeldActor);
	APlayerBistro* PlayerBistro = Cast<APlayerBistro>(UGameplayStatics::GetActorOfClass(GetWorld(), BP_PlayerBistro));

	// 샌드위치가 없는 빈 접시이거나 손님이 먹는 중이라면
 	if (Sandwich->Ingredients.Num() == 0 || SeatedCustomer->IsEat) {
		return;
	}
	Sandwich->SetIngrWidgetVisibility(ESlateVisibility::Hidden);

	TargetDiningTable->PutFoodOn(this, Sandwich);

	SeatedCustomer->EatSandwich();
	SeatedCustomer->AddPlayerSandwichReview(Sandwich);
}
