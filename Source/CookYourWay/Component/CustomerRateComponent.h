// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomerRateComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOKYOURWAY_API UCustomerRateComponent : public UActorComponent
{
	GENERATED_BODY()

	class AVillageManager* VillageManager;

	void Init();

public:	
	UCustomerRateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	const float MaxRate = 5.0;	// 최대 평점

	// <손님타입, 평균평점>
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, float> CustStringToRateMap;

	// <손님타입, 충성도>
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, int32> CustStringToRoyaltyMap;
};
