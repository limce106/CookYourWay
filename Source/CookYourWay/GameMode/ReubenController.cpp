// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ReubenController.h"
#include <Kismet/GameplayStatics.h>
#include <Widget/IngredientBoardWidget.h>

AReubenController::AReubenController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void AReubenController::BeginPlay()
{
	Super::BeginPlay();

	//FInputModeUIOnly InputModeData;
	//InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	//SetInputMode(InputModeData);

	Reuben = Cast<AReuben>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void AReubenController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this, &AReubenController::Interaction);
}

void AReubenController::Interaction()
{
	CookInteraction();
}

void AReubenController::CookInteraction()
{
	if (!Reuben->OverlappedActor) {
		return;
	}

	UClass* OverlappedActorClass = Reuben->OverlappedActor->GetClass();
	if (OverlappedActorClass == BP_Table) {
		ATable* Table = Cast<ATable>(Reuben->OverlappedActor);
		Table->TableInteraction();
	}
	else if (OverlappedActorClass == BP_Sandwich) {
		ASandwich* Sandwich = Cast<ASandwich>(Reuben->OverlappedActor);
		Sandwich->SandwichInteraction();
	}
	else if (OverlappedActorClass == BP_Ingredient) {
		AIngredient* Ingredient = Cast<AIngredient>(Reuben->OverlappedActor);
		Ingredient->IngredientInteraction();
	}
	else if (OverlappedActorClass == BP_FryPan) {
		AFryPan* FryPan = Cast<AFryPan>(Reuben->OverlappedActor);
		FryPan->FryPanInteraction();
	}
	else if (OverlappedActorClass == BP_CuttingBoard) {
		ACuttingBoard* CuttingBoard = Cast<ACuttingBoard>(Reuben->OverlappedActor);
		CuttingBoard->CuttingBoardInteraction();
	}
	else if (OverlappedActorClass == BP_Oven) {
		AOven* Oven = Cast<AOven>(Reuben->OverlappedActor);
		Oven->OvenInteraction();
	}
	else if (OverlappedActorClass == BP_DiningTable) {
		ADiningTable* DiningTable = Cast<ADiningTable>(Reuben->OverlappedActor);
		DiningTable->DiningTableInteraction();
	}
	else if (OverlappedActorClass == BP_Fridge) {
		FridgeInteraction();
	}
	else if (OverlappedActorClass == BP_Plates) {
		PlatesInteraction();
	}
	else if (OverlappedActorClass == BP_TrashBin) {
		TrashBinInteraction();
	}
}

void AReubenController::FridgeInteraction()
{
	UIngredientBoardWidget* BP_IngredientBoard = CreateWidget<UIngredientBoardWidget>(GetWorld(), BP_IngredientBoardClass);
	if (BP_IngredientBoard) {
		BP_IngredientBoard->AddToViewport();
	}
}

void AReubenController::PlatesInteraction()
{
	if (!Reuben->IsHold) {
		ASandwich* Sandwich = GetWorld()->SpawnActor<ASandwich>(BP_Sandwich, Reuben->GetActorLocation(), Reuben->GetActorRotation());
		Reuben->HoldActor(Sandwich);
	}
	else if (Reuben->IsHold && Reuben->GetHeldActorClass() == BP_Ingredient) {
		AIngredient* Ingredient = Cast<AIngredient>(Reuben->HeldActor);
		if (Ingredient->IsCooked()) {
			ASandwich* Sandwich = GetWorld()->SpawnActor<ASandwich>(BP_Sandwich, Reuben->GetActorLocation(), Reuben->GetActorRotation());
			Reuben->HoldActor(Sandwich);
			Sandwich->AddIngredient(Ingredient);
		}
	}
}

void AReubenController::TrashBinInteraction()
{
	if (!Reuben->IsHold) {
		return;
	}

	if (Reuben->GetHeldActorClass() == BP_Sandwich) {
		ASandwich* HeldSandwich = Cast<ASandwich>(Reuben->HeldActor);
		HeldSandwich->DestroySandwich();
	}
	else {
		Reuben->HeldActor->Destroy();
	}
	Reuben->IsHold = false;
}

