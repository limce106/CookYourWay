// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomerRateComponent.generated.h"

UENUM(BlueprintType)
enum class ECustType : uint8 {
	Amy		UMETA(DisplayName = "Amy"),
	Timmy	UMETA(DisplayName = "Timmy")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOKYOURWAY_API UCustomerRateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCustomerRateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	const float MaxRate = 5.0;	// 최대 평점

	UPROPERTY(BlueprintReadWrite)
	TMap<ECustType, float> CustTypeRateMap = {	// <손님타입, 평균평점>, 
		{ECustType::Amy, 0.0 },
		{ECustType::Timmy, 0.0 }
	};

	const TMap<FString, ECustType> CustStringToTypeMap = {
		{"Amy", ECustType::Amy},
		{"Timmy", ECustType::Timmy}
	};
};
