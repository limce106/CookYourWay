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
	FString CustomerType; // 손님 유형
	UPROPERTY(BlueprintReadWrite)
	int32 SpawnedArea; // 스폰된 구역번호

	virtual void Tick(float DeltaTime) override;

};
