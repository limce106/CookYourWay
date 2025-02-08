// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Sandwich.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/IngredientManagerSystem.h"
#include "Reuben.h"

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
	FRotator CurIngrRot;
	if (Ingredients.Num() == 0) {
		CurIngrLoc = GetActorLocation();
		CurIngrLoc.Z += 2.0f;
		CurIngrRot = GetActorRotation();
	}
	else {
		FVector LastIngrLoc = Ingredients[Ingredients.Num() - 1]->GetActorLocation();
		CurIngrLoc = FVector(LastIngrLoc.X, LastIngrLoc.Y, LastIngrLoc.Z + IngrHeight + 2.0f);
		CurIngrRot = Ingredients[Ingredients.Num() - 1]->GetActorRotation();
	}

	// 재료 부착
	Ingr->SetActorEnableCollision(false);
	Ingr->AttachToComponent(DefaultRootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Ingr->SetActorLocation(CurIngrLoc);
	Ingr->SetActorRotation(CurIngrRot);

	Ingredients.Add(Ingr);
	AddIngredientImg();
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

TArray<int32> ASandwich::IngrActorToNum()
{
	TArray<int32> IngrNum;

	for (int i = 0; i < Ingredients.Num(); i++) {
		int32 IngrIndex = *IngredientManagerSystem->IngrNameIndexMap.Find(Ingredients[i]->CurIngrData.IngrName);
		IngrNum.Add(IngrIndex);
	}

	return IngrNum;
}

bool ASandwich::IsMeatBurn() {
	for (auto OneIngredient : Ingredients) {
		if (OneIngredient->CurIngrData.IngrType == "Meat") {
			if (OneIngredient->IsBurn) {
				return true;
			}
			else {
				return false;
			}
		}
	}

	return false;
}

void ASandwich::SandwichInteraction()
{
	AReuben* Reuben = Cast<AReuben>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (!Reuben->IsHold) {
		Reuben->HoldActor(this);
	}
	// 조리도구 위에 조리 완료된 재료가 있다면 재료를 접시/샌드위치 위로 올린다.
	else if (Reuben->GetHeldActorClass() ==  ACookingUtensil::StaticClass()) {
		ACookingUtensil* HoldingCookingUtensil = Cast<ACookingUtensil>(Reuben->HeldActor);
		if (HoldingCookingUtensil->IsIngredientOn && HoldingCookingUtensil->PlacedIngredient->IsCooked()) {
			AddIngredient(HoldingCookingUtensil->PlacedIngredient);
		}
	}
	// 조리된 재료라면 접시/샌드위치 위로 올린다.
	else if (Reuben->GetHeldActorClass()->IsChildOf(AIngredient::StaticClass())) {
		AIngredient* HoldingIngr = Cast<AIngredient>(Reuben->HeldActor);
		if (HoldingIngr->IsCooked()) {
			AddIngredient(HoldingIngr);
		}
	}
}

bool ASandwich::IsFirstIngrBread() {
	if (Ingredients[0]->CurIngrData.IngrType == "Bread") {
		return true;
	}
	else {
		return false;
	}
}

bool ASandwich::IsLastIngrBread() {
	if (Ingredients.Num() > 0) {
		if (Ingredients[Ingredients.Num() - 1]->CurIngrData.IngrType == "Bread") {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}