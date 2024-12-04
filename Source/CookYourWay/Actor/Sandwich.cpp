// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Sandwich.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/IngredientManagerSystem.h"

ASandwich::ASandwich()
{
	PrimaryActorTick.bCanEverTick = true;

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
	FVector CurIngrLoc = GetActorLocation();
	if (Ingredients.Num() == 0) {
		LastIngrLocZ = GetActorLocation().Z;
	}

	// 재료 높이 계산
	FVector IngrBounds = Ingr->GetComponentsBoundingBox().GetExtent();
	float IngrHeight = IngrBounds.Z * 2;

	CurIngrLoc.Z = LastIngrLocZ + IngrHeight;
	LastIngrLocZ = CurIngrLoc.Z;

	// 재료 부착
	Ingr->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	Ingr->SetActorLocation(CurIngrLoc);

	int IngrIndex = *IngredientManagerSystem->IngrNameIndexMap.Find(Ingr->CurIngrData->IngrName);
	Ingredients.Add(IngrIndex);

	UE_LOG(LogTemp, Warning, TEXT("CurIngrLoc: %s"), *CurIngrLoc.ToString());
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

