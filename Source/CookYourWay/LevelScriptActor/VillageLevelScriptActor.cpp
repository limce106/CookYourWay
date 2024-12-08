// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelScriptActor/VillageLevelScriptActor.h"

void AVillageLevelScriptActor::Init()
{
	if (BP_PlayerBistroHUDClass) {
		UUserWidget* PlayerBistroHUD = CreateWidget<UUserWidget>(GetWorld(), BP_PlayerBistroHUDClass);
		PlayerBistroHUD->AddToViewport();
	}
}

void AVillageLevelScriptActor::BeginPlay()
{
	Init();
}

void AVillageLevelScriptActor::Tick(float DeltaTime)
{

}
