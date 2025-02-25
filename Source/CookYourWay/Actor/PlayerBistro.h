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

	// 테스트
	class UCustomerDataManagerSystem* CustomerDataManagerSystem;
	class UIngredientManagerSystem* IngredientManagerSystem;
	//
	class UVillageManagerSystem* VillageManagerSystem;
	ACustomerPool* CustomerPool;

	// 손님 위치
	TArray<FVector> CustSeatLocArr = {
		//FVector(0.0f, -5600.0f, 120.0f),
		FVector(150.0f, -5600.0f, 120.0f),
		FVector(300.0f, -5600.0f, 120.0f),
		FVector(450.0f, -5600.0f, 120.0f),
		FVector(600.0f, -5600.0f, 120.0f),
		FVector(750.0f, -5600.0f, 120.0f),
		//FVector(900.0f, -5600.0f, 120.0f)
	};

	// 자리에 손님이 앉았는지
	TArray<bool> IsSeated;
	// 대기 손님
	TQueue<FString> WaitingCustQueue;
	int32 WaitingCustNum = 0;
	// 대기 손님 인내심
	TArray<float> WaitingCustPatience;
	// 다음 손님이 앉기까지의 시간
	const float NextCustDelay = 1.5f;
	// 최대 대기 시간(초)
	const float MaxWaitingTime = 40;

	void SpawnDiningTable();

	// 손님 앉히기
	void SitCust(ACustomer* Customer, int32 SeatIdx);
	// 손님 대기시키기
	void WaitCust(ACustomer* Customer);
	// 손님을 앉히거나 대기시킴
	void SitOrWaitCust(ACustomer* Customer);
	// 빈자리 찾기
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

	// 부지 번호
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 AreaID;
	// 오늘 방문한 손님 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 TodayCust = 0;
	// 오늘 순 수익
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 TodayNetIncome = 0;
	// 오늘 판 가격
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 TodaySoldPrice = 0;
	// 꺼낸 재료들의 가격
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 UsedIngrPrice = 0;

	UFUNCTION(BlueprintCallable)
	int32 GetWaitingCustNum();

	// 손님이 방문했을 때
	UFUNCTION(BlueprintCallable)
	void CustomerVisited(ACustomer* Customer);

	void LeaveAndSitNextCust(ACustomer* LeftCustomer);

	void SitNextCust(int32 SeatIdx);

	void LeaveWaitingCust();

	ADiningTable* GetDiningTable(int32 SeatIdx);
	void LeaveCustomerInBistro(ACustomer* Customer);
};
