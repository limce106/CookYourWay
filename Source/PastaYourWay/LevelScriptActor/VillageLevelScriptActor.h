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

public:
	virtual void Tick(float DeltaTime) override;
};
