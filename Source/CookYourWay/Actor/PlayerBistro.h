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

	// 손님 대기 위치 지정
	UFUNCTION(BlueprintCallable)
	void SetCustWaitLoc(ACustomer* Customer);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 오늘 방문한 손님 수
	int32 VisitedCustNum = 0;
	// 오늘 손님의 평점 평균
	float CustomerReviewAvg = 0;

	// 손님이 방문했을 때
	UFUNCTION(BlueprintCallable)
	void CustomerVisited(ACustomer* Customer);

	// 전체 평점 평균 갱신
	void UpdateCustomerReviewAvg(int32 ReveiwRate);
};
