// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Sandwich.h"

ASandwich::ASandwich()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASandwich::BeginPlay()
{
	Super::BeginPlay();
	
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

	CurIngrLoc.Z += IngrHeight;
	Ingr->SetActorLocation(CurIngrLoc);

	// 재료 부착
	Ingr->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	Ingredients.Add(Ingr);
}

//void ASandwich::DetachOneIngr(AIngredient* Ingr)
//{
//	if (Ingredients.Num() == 1) {
//		Ingr->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//		Ingredients.RemoveAt(0);
//	}
//}

