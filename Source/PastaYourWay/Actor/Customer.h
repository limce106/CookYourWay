// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Customer.generated.h"

UCLASS()
class PASTAYOURWAY_API ACustomer : public APawn
{
	GENERATED_BODY()

public:
	ACustomer();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite)
	FString CustomerType; // �մ� ����
	UPROPERTY(BlueprintReadWrite)
	int32 SpawnedArea; // ������ ������ȣ

	virtual void Tick(float DeltaTime) override;

};
