// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/CookingUtensil.h"
#include "FryPan.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API AFryPan : public ACookingUtensil
{
	GENERATED_BODY()
	
protected:
	float GetOneCookIncreasement() override;

public:
	virtual void Tick(float DeltaTime) override;

	bool IsFrying = false;
	void Fry();
};
