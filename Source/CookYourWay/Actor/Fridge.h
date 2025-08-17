// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "Fridge.generated.h"

UCLASS()
class COOKYOURWAY_API AFridge : public AActor, public IInteractable
{
	GENERATED_BODY()
	class AReuben* Reuben;
	
public:	
	AFridge();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> BP_FridgeWidget;

	virtual void Interact_Implementation() override;
};
