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

	// 손님 위치
	TArray<FVector> CustSeatLocArr = {
		/*추후 지정 필요*/
		FVector(440.0f, -5450.0f, 120.0f),
		FVector(640.0f, -5450.0f, 120.0f),
		FVector(840.0f, -5450.0f, 120.0f),
		FVector(1040.0f, -5450.0f, 120.0f),
		FVector(1240.0f, -5450.0f, 120.0f)
	};

	// 자리에 손님이 앉았는지
	TArray<bool> IsSeated;
	// 대기 손님
	TQueue<AActor*> WaitingCustQueue;
	int32 WaitingCustNum = 0;
	// 다음 손님이 앉기까지의 시간
	const float NextCustDelay = 1.5f;


	void SpawnDiningTable();
	ADiningTable* GetDiningTable(int32 SeatIdx);

	// 손님 앉히기
	void SitCust(ACustomer* Customer, int32 SeatIdx);
	// 손님 대기시키기
	void WaitCust(ACustomer* Customer);
	// 손님을 앉히거나 대기시킴
	void SitOrWaitCust(ACustomer* Customer);
	// 빈자리 찾기
	int32 FindEmptySeatIdx();
	
public:	
	APlayerBistro();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rate)
	class UCustomerRateComponent* CustRateComponent;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 오늘 방문한 손님 수
	int32 VisitedCustNum = 0;
	// 오늘 손님의 평점 평균
	float CustomerReviewAvg = 0;

	UFUNCTION(BlueprintCallable)
	int32 GetWaitingCustNum();

	// 손님이 방문했을 때
	UFUNCTION(BlueprintCallable)
	void CustomerVisited(ACustomer* Customer);

	// 전체 평점 평균 갱신
	void UpdateCustomerReviewAvg(int32 ReveiwRate);

	void LeaveAndSitNextCust(ACustomer* LeftCustomer);

	void SitNextCust(int32 SeatIdx);

	void LeaveWaitingCust(ACustomer* Customer);
};
