// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Component/CustomerRateComponent.h"
#include "Competitor.generated.h"

UCLASS()
class COOKYOURWAY_API ACompetitor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACompetitor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetCustRate(ACustomer* Customer);

public:	
	virtual void Tick(float DeltaTime) override;

};
