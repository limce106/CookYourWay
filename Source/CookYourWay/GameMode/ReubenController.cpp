// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ReubenController.h"

AReubenController::AReubenController()
{
	
}

void AReubenController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this, &AReubenController::Interaction);
}

void AReubenController::Interaction()
{
	BP_Interaction();
}