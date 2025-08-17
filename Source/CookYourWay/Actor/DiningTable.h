// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Customer.h"
#include "DiningTable.generated.h"

UCLASS()
class COOKYOURWAY_API ADiningTable : public AActor, public IInteractable
{
	GENERATED_BODY()
	
	ADiningTable();
	class AReuben* Reuben;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACustomer* SeatedCustomer;

	// 테이블 위에 액터가 있는지
	UPROPERTY(BlueprintReadWrite)
	bool IsActorOn = false;
	// 테이블 위에 올라간 액터
	UPROPERTY(BlueprintReadWrite)
	AActor* PlacedActor;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Sandwich;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Dessert;

	UFUNCTION()
	void DestroyFoodOnDiningTable();

	virtual void Interact_Implementation() override;
};
