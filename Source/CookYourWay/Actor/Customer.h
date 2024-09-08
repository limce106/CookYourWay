// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Customer.generated.h"

UCLASS()
class COOKYOURWAY_API ACustomer : public APawn
{
	GENERATED_BODY()

public:
	ACustomer();

	UPROPERTY(BlueprintReadWrite)
	bool IsWalk = true;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
