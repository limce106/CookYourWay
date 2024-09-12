// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Component/CustomerRateComponent.h"
#include "PlayerBistro.generated.h"

UCLASS()
class COOKYOURWAY_API APlayerBistro : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayerBistro();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetCustWaitLoc(ACustomer* Customer);	// 손님 대기 위치 지정

public:	
	virtual void Tick(float DeltaTime) override;

};
