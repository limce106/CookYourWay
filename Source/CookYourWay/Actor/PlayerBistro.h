// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DiningTable.h"
#include "GameInstance/CustomerDataManagerSystem.h"
#include "CustomerPool.h"
#include "PlayerBistro.generated.h"

UCLASS()
class COOKYOURWAY_API APlayerBistro : public AActor
{
	GENERATED_BODY()

	// �׽�Ʈ
	class UCustomerDataManagerSystem* CustomerDataManagerSystem;
	class UIngredientManagerSystem* IngredientManagerSystem;
	//
	class UVillageManagerSystem* VillageManagerSystem;
	ACustomerPool* CustomerPool;

	// �մ� ��ġ
	TArray<FVector> CustSeatLocArr = {
		//FVector(0.0f, -5600.0f, 120.0f),
		FVector(150.0f, -5600.0f, 120.0f),
		FVector(300.0f, -5600.0f, 120.0f),
		FVector(450.0f, -5600.0f, 120.0f),
		FVector(600.0f, -5600.0f, 120.0f),
		FVector(750.0f, -5600.0f, 120.0f),
		//FVector(900.0f, -5600.0f, 120.0f)
	};

	// �ڸ��� �մ��� �ɾҴ���
	TArray<bool> IsSeated;
	// ��� �մ�
	TQueue<FString> WaitingCustQueue;
	int32 WaitingCustNum = 0;
	// ��� �մ� �γ���
	TArray<float> WaitingCustPatience;
	// ���� �մ��� �ɱ������ �ð�
	const float NextCustDelay = 1.5f;
	// �ִ� ��� �ð�(��)
	const float MaxWaitingTime = 40;

	void SpawnDiningTable();

	// �մ� ������
	void SitCust(ACustomer* Customer, int32 SeatIdx);
	// �մ� ����Ű��
	void WaitCust(ACustomer* Customer);
	// �մ��� �����ų� ����Ŵ
	void SitOrWaitCust(ACustomer* Customer);
	// ���ڸ� ã��
	int32 FindEmptySeatIdx();

	float TempDelayTime;
	bool DelayWithDeltaTime(float DelayTime, float DeltaSeconds);
	void DecreaseWaitingCustPatience();
	
public:	
	APlayerBistro();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADiningTable> BP_DiningTable;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACustomer> BP_Customer;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACustomerPool> BP_CustomerPool;

	// ���� ��ȣ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 AreaID;
	// ���� �湮�� �մ� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 TodayCust = 0;
	// ���� �� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 TodayNetIncome = 0;
	// ���� �� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 TodaySoldPrice = 0;
	// ���� ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 UsedIngrPrice = 0;

	UFUNCTION(BlueprintCallable)
	int32 GetWaitingCustNum();

	// �մ��� �湮���� ��
	UFUNCTION(BlueprintCallable)
	void CustomerVisited(ACustomer* Customer);

	void LeaveAndSitNextCust(ACustomer* LeftCustomer);

	void SitNextCust(int32 SeatIdx);

	void LeaveWaitingCust();

	ADiningTable* GetDiningTable(int32 SeatIdx);
	void LeaveCustomerInBistro(ACustomer* Customer);
};
