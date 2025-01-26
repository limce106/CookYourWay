// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NewsEffectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOKYOURWAY_API UNewsEffectComponent : public UActorComponent
{
	GENERATED_BODY()

	UNewsEffectComponent();

	class UCustomerDataManagerSystem* CustomerDataManagerSystem;
	class UVillageManagerSystem* VillageManagerSystem;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FString CurNewsEffect;
	FString CurNewsKeyWord;

	// 감소된 손님 생성 확률
	const float DecreasedSpawnCustPercent = 0.2;
	// 증가된 손님 생성 확률
	const float IncreasedSpwanCustPercent = 0.7;

	// 감소된 손님 생성 확률을 반영한 전체 손님 생성 확률
	TArray<float> GetDecSpawnCustPercent(TArray<FString> CustName);
	// 증가된 손님 생성 확률을 반영한 전체 손님 생성 확률
	TArray<float> GetIncSpawnCustPercent(TArray<FString> CustName);

	// 증가된 손님 최소 스폰 주기
	const int32 IncreaseSpawnCustMin = 3;
	// 증가된 손님 최대 스폰 주기
	const int32 IncreaseSpawnCustMax = 5;
};
