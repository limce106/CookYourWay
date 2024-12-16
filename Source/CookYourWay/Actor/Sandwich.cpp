// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Sandwich.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/IngredientManagerSystem.h"

ASandwich::ASandwich()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootComponent"));
	RootComponent = DefaultRootComponent;
}

void ASandwich::BeginPlay()
{
	Super::BeginPlay();
	
	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();
}

void ASandwich::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASandwich::AddIngredient(AIngredient* Ingr)
{
	// 재료 높이 계산
	FVector IngrBounds = Ingr->GetComponentsBoundingBox().GetExtent();
	float IngrHeight = IngrBounds.Z * 2;

	FVector CurIngrLoc;
	if (Ingredients.Num() == 0) {
		CurIngrLoc = GetActorLocation();
	}
	else {
		CurIngrLoc = Ingredients[Ingredients.Num() - 1]->GetActorLocation();
	}
	CurIngrLoc.Z += IngrHeight;

	// 재료 부착
	// Ingr->SetActorEnableCollision(false);
	Ingr->AttachToComponent(DefaultRootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Ingr->SetActorLocation(CurIngrLoc);

	FRotator IngrRotation = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorRotation();
	Ingr->SetActorRotation(IngrRotation);

	Ingredients.Add(Ingr);
}

void ASandwich::DestroySandwich()
{
	TArray<AActor*> AttachedIngrs;
	GetAttachedActors(AttachedIngrs);
	for (auto Ingr : AttachedIngrs) {
		Ingr->Destroy();
	}

	Destroy();
}

//void ASandwich::DetachOneIngr(AIngredient* Ingr)
//{
//	if (Ingredients.Num() == 1) {
//		Ingr->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//		Ingredients.RemoveAt(0);
//	}
//}

TArray<int32> ASandwich::IngrActorToNum()
{
	TArray<int32> IngrNum;

	for (int i = 0; i < Ingredients.Num(); i++) {
		int IngrIndex = *IngredientManagerSystem->IngrNameIndexMap.Find(Ingredients[i]->CurIngrData->IngrName);
		IngrNum.Add(IngrIndex);
	}

	return IngrNum;
}