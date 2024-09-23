// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CookingUtensil.generated.h"

UCLASS()
class COOKYOURWAY_API ACookingUtensil : public AActor
{
	GENERATED_BODY()
	
public:	
	ACookingUtensil();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	bool IsFoodOn = false;
};
