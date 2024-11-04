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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rate)
	class UCustomerRateComponent* CustRateComponent;

	// �մ� ��� ��ġ ����
	UFUNCTION(BlueprintCallable)
	void SetCustWaitLoc(ACustomer* Customer);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// ���� �湮�� �մ� ��
	int32 VisitedCustNum = 0;
	// ���� �մ��� ���� ���
	float CustomerReviewAvg = 0;

	// �մ��� �湮���� ��
	UFUNCTION(BlueprintCallable)
	void CustomerVisited(ACustomer* Customer);

	// ��ü ���� ��� ����
	void UpdateCustomerReviewAvg(int32 ReveiwRate);
};
