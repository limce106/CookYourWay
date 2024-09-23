// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Reuben.generated.h"

UCLASS()
class COOKYOURWAY_API AReuben : public ACharacter
{
	GENERATED_BODY()

public:
	AReuben();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite)
	bool IsCarry = false;

	void MoveForward(float Value);
	void MoveRight(float Value);
};
