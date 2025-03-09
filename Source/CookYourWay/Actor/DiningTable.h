// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Customer.h"
#include "DiningTable.generated.h"

UCLASS()
class COOKYOURWAY_API ADiningTable : public AActor
{
	GENERATED_BODY()
	
	ADiningTable();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACustomer* SeatedCustomer;

	// ���̺� ���� ���Ͱ� �ִ���
	UPROPERTY(BlueprintReadWrite)
	bool IsActorOn = false;
	// ���̺� ���� �ö� ����
	UPROPERTY(BlueprintReadWrite)
	AActor* PlacedActor;

	UFUNCTION()
	void DiningTableInteraction();
	UFUNCTION()
	void PutFoodOn(AActor* HoldingCharacter, AActor* Food);
	UFUNCTION()
	void DestroyFoodOnDiningTable();
};
