// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Component/CustomerRateComponent.h"
#include "GameInstance/VillageManagerSystem.h"
#include "Customer.h"
#include "Competitor.generated.h"

UCLASS()
class COOKYOURWAY_API ACompetitor : public AActor
{
	GENERATED_BODY()
	
	class UVillageManagerSystem* VillageManagerSystem;

	void SetDefaultReviewRate();
	
	// 전체 평점 평균 갱신
	void UpdateCustomerReviewAvg(int32 ReveiwRate);
	
public:	
	ACompetitor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rate)
	class UCustomerRateComponent* CustRateComponent;

	TArray<FCompetitorReviewData> NormalReviewData;
	TArray<FCompetitorReviewData> IngrFestReviewData;
	TArray<FCompetitorReviewData> OpenPromoReviewData;

	// 오늘 방문한 손님 수
	int32 VisitedCustNum = 0;
	// 오늘 손님의 평점 평균
	float CustomerReviewAvg = 0;
	// 새로운 손님의 평점 평균 가져오기
	int32 GetCustomerReview();

protected:
	virtual void BeginPlay() override;

	// 손님이 방문했을 때
	UFUNCTION(BlueprintCallable)
	void CustomerVisited(ACustomer* Customer);

public:	
	virtual void Tick(float DeltaTime) override;

};
