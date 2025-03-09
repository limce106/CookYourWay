// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dessert.generated.h"

UCLASS()
class COOKYOURWAY_API ADessert : public AActor
{
	GENERATED_BODY()
	
public:	
	ADessert();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	bool IsCooked = false;
};
