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

	// ���ҵ� �մ� ���� Ȯ��
	const float DecreasedSpawnCustPercent = 0.2;
	// ������ �մ� ���� Ȯ��
	const float IncreasedSpwanCustPercent = 0.7;

	// ���ҵ� �մ� ���� Ȯ���� �ݿ��� ��ü �մ� ���� Ȯ��
	TArray<float> GetDecSpawnCustPercent(TArray<FString> CustName);
	// ������ �մ� ���� Ȯ���� �ݿ��� ��ü �մ� ���� Ȯ��
	TArray<float> GetIncSpawnCustPercent(TArray<FString> CustName);

	// ������ �մ� �ּ� ���� �ֱ�
	const int32 IncreaseSpawnCustMin = 3;
	// ������ �մ� �ִ� ���� �ֱ�
	const int32 IncreaseSpawnCustMax = 5;
};
