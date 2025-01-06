// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelScriptActor/AuctionLevelScriptActor.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

void AAuctionLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuctionLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAuctionLevelScriptActor::SetAuctionItems()
{
	UIngredientManagerSystem* IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();
	int32 ItemNum = 6;
	int32 SClassProb = UKismetMathLibrary::RandomIntegerInRange(1, 100);

	if (SClassProb <= 10) {
		int32 SClassRandIdx = UKismetMathLibrary::RandomIntegerInRange(1, IngredientManagerSystem->SClassIngrRows.Num() - 1);
		AuctionIngrTableIdxArr.Add(IngredientManagerSystem->SClassIngrRows[SClassRandIdx]);
		ItemNum--;
		IsContainSClass = true;
	}

	TArray<int32> ABClassIngr;
	ABClassIngr += IngredientManagerSystem->AClassIngrRows;
	ABClassIngr += IngredientManagerSystem->BClassIngrRows;
	
	for (int i = 0; i < ItemNum; i++) {
		int32 ItemIdx = UKismetMathLibrary::RandomIntegerInRange(1, ABClassIngr.Num() - 1);
		AuctionIngrTableIdxArr.Add(ABClassIngr[ItemIdx]);
		ABClassIngr.RemoveAt(ItemIdx);
	}
}
