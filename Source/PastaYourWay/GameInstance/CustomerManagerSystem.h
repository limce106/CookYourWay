// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CustomerManagerSystem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PASTAYOURWAY_API UCustomerManagerSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	TArray<int32> Area = { 1, 2, 3, 4, 5, 6,
							7, 8, 9, 10, 11, 12,
							13,14, 15, 16, 17, 18,
							19, 20, 21, 22, 23, 24,
							25, 26, 27, 28, 29, 30,
							31, 32, 33, 34, 35, 36 };
};
