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
	Reuben = Cast<AReuben>(UGameplayStatics::GetPlayerPawn(this, 0));
	PreviewSandwich = Cast<APreviewSandwich>(UGameplayStatics::GetActorOfClass(GetWorld(), APreviewSandwich::StaticClass()));
}

void ASandwich::AddIngredient(AIngredient* Ingr)
{
	Ingr->SetActorEnableCollision(false);
	// 재료 높이 계산
	FVector IngrBounds = Ingr->GetComponentsBoundingBox().GetExtent();
	float IngrHeight = IngrBounds.Z ;

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
	Ingr->AttachToComponent(DefaultRootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	Ingr->SetActorLocation(CurIngrLoc);
	Ingr->SetActorRotation(CurIngrRot);

	Ingredients.Add(Ingr);
	AddIngredientImg();

	ShowPreviewSandwich();
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

TArray<int32> ASandwich::GetIngrNum()
{
	TArray<int32> IngrNum;

	for (int i = 0; i < Ingredients.Num(); i++) {
		int32 IngrIndex = *IngredientManagerSystem->IngrNameIndexMap.Find(Ingredients[i]->CurIngrData.IngrName);
		IngrNum.Add(IngrIndex);
	}

	return IngrNum;
}

bool ASandwich::IsMeatBurn()
{
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

bool ASandwich::IsFirstIngrBread()
{
	if (Ingredients.Num() > 0) {
		if (Ingredients[0]->CurIngrData.IngrType == "Bread") {
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

bool ASandwich::IsLastIngrBread()
{
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

bool ASandwich::IsCompleteSandwich()
{
	if (Ingredients.Num() >= 2) {
		return IsFirstIngrBread() && IsLastIngrBread();
	}
	else {
		return false;
	}
}

void ASandwich::ShowPreviewSandwich()
{
	if (Reuben->HeldActor == this)
	{
		TArray<AActor*> IngredientActors;
		for (AIngredient* I : Ingredients)
		{
			IngredientActors.Add(I);
		}

		PreviewSandwich->SetIngredients(IngredientActors);
	}

	SetPreviewVisibility();
}

void ASandwich::OnPutDown_Implementation(AActor* PlaceTarget)
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

void ASandwich::OnPickUp_Implementation()
{
	Reuben->HoldActor(this);

	if(Ingredients.Num() > 0)
		ShowPreviewSandwich();

	USoundBase* LoadedSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Assets/Sound/SoundAsset/SFX_Grab.SFX_Grab"));
	UGameplayStatics::PlaySoundAtLocation(this, LoadedSound, GetActorLocation());
}