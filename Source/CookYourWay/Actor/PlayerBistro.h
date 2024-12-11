// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Component/CustomerRateComponent.h"
#include "DiningTable.h"
#include "PlayerBistro.generated.h"

UCLASS()
class COOKYOURWAY_API APlayerBistro : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADiningTable> BP_DiningTable;

	class UVillageManagerSystem* VillageManagerSystem;

	// �մ� ��ġ
	TArray<FVector> CustSeatLocArr = {
		/*���� ���� �ʿ�*/
		FVector(440.0f, -5450.0f, 120.0f),
		FVector(640.0f, -5450.0f, 120.0f),
		FVector(840.0f, -5450.0f, 120.0f),
		FVector(1040.0f, -5450.0f, 120.0f),
		FVector(1240.0f, -5450.0f, 120.0f)
	};

	// �ڸ��� �մ��� �ɾҴ���
	TArray<bool> IsSeated;
	// ��� �մ�
	TQueue<AActor*> WaitingCustQueue;
	int32 WaitingCustNum = 0;
	// ���� �մ��� �ɱ������ �ð�
	const float NextCustDelay = 1.5f;


	void SpawnDiningTable();
	ADiningTable* GetDiningTable(int32 SeatIdx);

	// �մ� ������
	void SitCust(ACustomer* Customer, int32 SeatIdx);
	// �մ� ����Ű��
	void WaitCust(ACustomer* Customer);
	// �մ��� �����ų� ����Ŵ
	void SitOrWaitCust(ACustomer* Customer);
	// ���ڸ� ã��
	int32 FindEmptySeatIdx();
	
public:	
	APlayerBistro();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rate)
	class UCustomerRateComponent* CustRateComponent;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// ���� �湮�� �մ� ��
	int32 VisitedCustNum = 0;
	// ���� �մ��� ���� ���
	float CustomerReviewAvg = 0;

	UFUNCTION(BlueprintCallable)
	int32 GetWaitingCustNum();

	// �մ��� �湮���� ��
	UFUNCTION(BlueprintCallable)
	void CustomerVisited(ACustomer* Customer);

	// ��ü ���� ��� ����
	void UpdateCustomerReviewAvg(int32 ReveiwRate);

	void LeaveAndSitNextCust(ACustomer* LeftCustomer);

	void SitNextCust(int32 SeatIdx);

	void LeaveWaitingCust(ACustomer* Customer);
};
