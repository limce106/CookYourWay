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
	
	// ��ü ���� ��� ����
	void UpdateCustomerReviewAvg(int32 ReveiwRate);
	
public:	
	ACompetitor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rate)
	class UCustomerRateComponent* CustRateComponent;

	TArray<FCompetitorReviewData> NormalReviewData;
	TArray<FCompetitorReviewData> IngrFestReviewData;
	TArray<FCompetitorReviewData> OpenPromoReviewData;

	// ���� �湮�� �մ� ��
	int32 VisitedCustNum = 0;
	// ���� �մ��� ���� ���
	float CustomerReviewAvg = 0;
	// ���ο� �մ��� ���� ��� ��������
	int32 GetCustomerReview();

protected:
	virtual void BeginPlay() override;

	// �մ��� �湮���� ��
	UFUNCTION(BlueprintCallable)
	void CustomerVisited(ACustomer* Customer);

public:	
	virtual void Tick(float DeltaTime) override;

};
