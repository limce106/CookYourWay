// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameInstance/VillageManagerSystem.h"
#include "GameInstance/CustomerDataManagerSystem.h"
#include "Customer.h"
#include "Competitor.generated.h"

UCLASS()
class COOKYOURWAY_API ACompetitor : public AActor
{
	GENERATED_BODY()

	ACompetitor();
	
	class UVillageManagerSystem* VillageManagerSystem;
	class UCustomerDataManagerSystem* CustomerDataManagerSystem;

	void SetDefaultReviewRate();
	void UpdateTotalCustAndRateSum();

protected:
	virtual void BeginPlay() override;

	// 손님이 방문했을 때
	UFUNCTION(BlueprintCallable)
	void CustomerVisited(ACustomer* Customer);

public:	
	virtual void Tick(float DeltaTime) override;

	TArray<FCompetitorReviewData> NormalReviewData;
	TArray<FCompetitorReviewData> IngrFestReviewData;
	TArray<FCompetitorReviewData> OpenPromoReviewData;

	bool IsOpenPromo;

	// 부지 번호
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 AreaID;

	// 새로운 손님의 평점 평균 가져오기
	int32 GetCustomerSatisfaction();
};