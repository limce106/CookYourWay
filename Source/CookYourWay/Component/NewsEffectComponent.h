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

	UPROPERTY()
	FString CurNewsEffect;
	UPROPERTY()
	FString CurNewsKeyWord;

	// ���ҵ� �մ� ���� Ȯ��
	const float DecreasedSpawnCustProb = 0.2;
	// ������ �մ� ���� Ȯ��
	const float IncreasedSpwanCustProb = 0.7;

	// ���ҵ� �մ� ���� Ȯ���� �ݿ��� ��ü �մ� ���� Ȯ��
	UFUNCTION()
	TArray<float> GetDecSpawnCustProb(TArray<FString> CustName);
	// ������ �մ� ���� Ȯ���� �ݿ��� ��ü �մ� ���� Ȯ��
	UFUNCTION()
	TArray<float> GetIncSpawnCustProb(TArray<FString> CustName);

	// ������ �մ� �ּ� ���� �ֱ�)
	const int32 IncreaseSpawnCustMin = 3;
	// ������ �մ� �ִ� ���� �ֱ�
	const int32 IncreaseSpawnCustMax = 5;

	UFUNCTION()
	void CustNameKorToEng();
};
