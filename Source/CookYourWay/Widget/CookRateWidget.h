// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "CookRateWidget.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API UCookRateWidget : public UUserWidget
{
	GENERATED_BODY()

	void NativePreConstruct();
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* ProgressBar_CookRate;

	UPROPERTY(BlueprintReadOnly)
	float CookRate = 0.0f;
};
