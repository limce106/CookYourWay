// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerBistro.h"
#include "PartTimer.generated.h"

UCLASS()
class COOKYOURWAY_API APartTimer : public ACharacter
{
	GENERATED_BODY()

	void SetHeldActorLoc();

public:
	APartTimer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlayerBistro> BP_PlayerBistro;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* HeldActor;
	UFUNCTION(BlueprintCallable)
	void HoldActor(AActor* Actor);
	UFUNCTION(BlueprintCallable)
	void PutDownActor();

	UFUNCTION(BlueprintCallable)
	TArray<int32> GetHoldingSandwichIngr();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ADiningTable* TargetDiningTable;
	UFUNCTION(BlueprintCallable)
	void GiveSandwich();
};
