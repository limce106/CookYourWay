// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Component/CustomerRateComponent.h"
#include "GameInstance/VillageManagerSystem.h"
#include "Competitor.generated.h"

UCLASS()
class COOKYOURWAY_API ACompetitor : public AActor
{
	GENERATED_BODY()
	
	class UVillageManagerSystem* VillageManagerSystem;

	void SetDefaultReviewRate();
	
public:	
	ACompetitor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rate)
	class UCustomerRateComponent* CustRateComponent;

	TArray<FCompetitorReviewData> NormalReviewData;
	TArray<FCompetitorReviewData> IngrFestReviewData;
	TArray<FCompetitorReviewData> OpenPromoReviewData;

	// ¿À´Ã ¹æ¹®ÇÑ ¼Õ´Ô ¼ö
	int VisitedCustNum = 0;
	// ¿À´Ã ¼Õ´ÔÀÇ ÆòÁ¡ Æò±Õ
	float CustomerReviewAvg = 0;
	// »õ·Î¿î ¼Õ´ÔÀÇ ÆòÁ¡ Æò±Õ °¡Á®¿À±â
	int GetCustomerReview();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void CustomerVisited(ACustomer* Customer);

public:	
	virtual void Tick(float DeltaTime) override;

};
