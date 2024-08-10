// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"

#include "PastaYourWay/Actor/Customer.h"

#include "VillageLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class PASTAYOURWAY_API AVillageLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

	int32 TempTime;

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_CustomerClass;
	UPROPERTY(EditAnyWhere)
	AActor* Customer;

	virtual void Tick(float DeltaTime) override;

	bool DelayWithDeltaTime(float Sec);
	
};
