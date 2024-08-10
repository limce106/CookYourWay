// Fill out your copyright notice in the Description page of Project Settings.


#include "VillageLevelScriptActor.h"

void AVillageLevelScriptActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DelayWithDeltaTime(30)) {
		Customer = GetWorld()->SpawnActor<ACustomer>(BP_CustomerClass, FVector(1600.0f, 760.0f, -850.0f), FRotator(0.0f, 0.0f, 0.0f));
	}
}

bool AVillageLevelScriptActor::DelayWithDeltaTime(float Sec)
{	
	if (TempTime < Sec) {
		TempTime++;
		return false;
	}
	else {
		return true;
	}
}
