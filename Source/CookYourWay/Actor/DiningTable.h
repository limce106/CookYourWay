// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Customer.h"
#include "DiningTable.generated.h"

UCLASS()
class COOKYOURWAY_API ADiningTable : public AActor
{
	GENERATED_BODY()
	
	ADiningTable();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	ACustomer* SeatedCustomer;

	void DiningTableInteraction();
};
