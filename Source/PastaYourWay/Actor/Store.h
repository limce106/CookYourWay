// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PastaYourWay/Actor/Customer.h"

#include "Store.generated.h"

UCLASS()
class PASTAYOURWAY_API AStore : public AActor
{
	GENERATED_BODY()

	int32 TempTime;
	
public:	
	AStore();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_CustomerClass;
	UPROPERTY(EditAnyWhere)
	ACustomer* Customer;

	virtual void Tick(float DeltaTime) override;

	bool DelayWithDeltaTime(float DelayTime, float DeltaSeconds);
	void CreateCustomer();
};
