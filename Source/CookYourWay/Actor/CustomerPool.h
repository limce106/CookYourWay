// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Customer.h"
#include "CustomerPool.generated.h"

UCLASS()
class COOKYOURWAY_API ACustomerPool : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACustomer> BP_Customer;

	UPROPERTY()
	TArray<ACustomer*> CustomerPool;

	UPROPERTY(EditAnyWhere)
	int32 PoolSize = 15;
	
public:	
	ACustomerPool();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void InitCustomerPool();
	UFUNCTION()
	ACustomer* GetPooledCustomer(FString CustName, bool IsWalk);
	UFUNCTION()
	void ReturnCustomer(ACustomer* Customer);
};
