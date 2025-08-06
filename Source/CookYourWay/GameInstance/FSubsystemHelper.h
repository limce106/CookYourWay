// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class COOKYOURWAY_API FSubsystemHelper
{
public:
    template<typename T>
    static T* Get(UObject* WorldContext)
    {
        if (!WorldContext) return nullptr;

        UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContext);
        return GameInstance ? GameInstance->GetSubsystem<T>() : nullptr;
    }
};
