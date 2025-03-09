// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameInstance/VillageManagerSystem.h"
#include "GameInstance/CustomerDataManagerSystem.h"
#include "GameInstance/IngredientManagerSystem.h"
#include "Customer.h"
#include "Competitor.generated.h"

UCLASS()
class COOKYOURWAY_API ACompetitor : public AActor
{
	GENERATED_BODY()

	ACompetitor();
	class UNewsEffectComponent* NewsEffectComponent;
	
	class UVillageManagerSystem* VillageManagerSystem;
	class UCustomerDataManagerSystem* CustomerDataManagerSystem;
	class UIngredientManagerSystem* IngredientManagerSystem;
	class AVillageManager* VillageManager;

	TArray<FCompetitorReviewData> GetCompetitorReviewDataOnTable(FString DataType);
	void SetDefaultReviewRate();
	void UpdateCompetitorRating(float Rating);
	void AddRatingData(FString CustName, float Rating);

	bool IsCustTasteContainFestivalIngr(FString CustName);

	TArray<int32> GenerateSandwich(ACustomer* Customer);
	int32 GetCustomerSatisfaction(ACustomer* Customer, TArray<int32> Ingr);

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

	FTimerHandle CustRatingTimerHandler;

	FCompetitorData GetCurComptitorData();
};