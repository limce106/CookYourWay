// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "Sink.generated.h"

UCLASS()
class COOKYOURWAY_API ASink : public AActor, public IInteractable
{
	GENERATED_BODY()
	class AReuben* Reuben;
	
public:	
	ASink();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Sandwich;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Ingredient;

	virtual void Interact_Implementation() override;
};
