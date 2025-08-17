// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Holdable.h"
#include "Dessert.generated.h"

UCLASS()
class COOKYOURWAY_API ADessert : public AActor, public IHoldable
{
	GENERATED_BODY()

	class AReuben* Reuben;
	
public:	
	ADessert();

	const float TableZOffset = 103.f;
	const float DiningTableZOffset = 103.f;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	bool IsCooked = false;

	virtual void OnPutDown_Implementation(AActor* PlaceTarget) override;
	virtual void OnPickUp_Implementation() override;
};
