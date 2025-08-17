// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ReubenController.h"
#include <Kismet/GameplayStatics.h>
#include <Widget/FridgeWidget.h>
#include "Interface/Interactable.h"
#include "Interface/Holdable.h"

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
	Reuben->UpdateOverlappingActor();
	AActor* Target = Reuben->OverlappedActor;
	if (!Target) return;

	IInteractable::Execute_Interact(Target);
}
