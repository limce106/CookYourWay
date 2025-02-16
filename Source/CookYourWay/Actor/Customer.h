// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Ingredient.h"
#include "Sandwich.h"
#include "GameInstance/CustomerDataManagerSystem.h"
#include "Customer.generated.h"

UCLASS()
class COOKYOURWAY_API ACustomer : public ACharacter
{
	GENERATED_BODY()

	class AVillageManager* VillageManager;
	class UVillageManagerSystem* VillageManagerSystem;
	class UIngredientManagerSystem* IngredientManagerSystem;
	class UCustomerDataManagerSystem* CustomerDataManagerSystem;

	TArray<AActor*> AllCompetitorActorArr;
	class APlayerBistro* PlayerBistro;

	TMap <FVector, float> BistroLocRankMap;	// 가게 도착 위치와 계산된 점수

	// 식사 시간이 3초가 지난 시점부터 디저트를 받을 수 있다.
	const float CanGetDessertTime = 3.0f;
	// 최대 대기 시간(초)
	const float MaxWaitingTime = 40;

	// 판 전체 가격
	int32 TotalPaidPrice = 0;

	// 손님의 플레이어 가게에 대한 평가
	FPlayerBistroRatingData PlayerBistroRatingData;

	void SetSkeletalMesh();

	// 멘해튼 거리 구하기
	float ManhattanDist(FVector Loc1, FVector Loc2);
	// 방문 우선순위 구하기
	float CalcVisitRank(AActor* Bistro);
	// 방문 우선순위에 따라 목적지 설정
	FVector GetDestByVisitRank();
	// 충성도에 따라 목적지 설정
	bool GetDestByLoyalty();
	// 방문할 목적지(가게) 설정
	void SetVisitDest();

	float TempDelayTime;
	bool DelayWithDeltaTime(float DelayTime, float DeltaSeconds);

public:
	ACustomer();

protected:
	virtual void BeginPlay() override;

	// 손님 제거 타이머
	bool DestroyTimer = false;

	// 먹기 시작한 시간
	float StartEatTime = 0.0f;
	// 손님의 평점
	int32 Satisfaction = 0;

public:	
	virtual void Tick(float DeltaTime) override;
	
	void Init(FString Name, bool bWalk);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Competitor;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_PlayerBistro;

	UPROPERTY(BlueprintReadOnly)
	FString CustName;
	// 먹는 중인지
	UPROPERTY(BlueprintReadOnly)
	bool IsEat = false;
	UPROPERTY(BlueprintReadWrite)
	bool IsWalk = false;
	UPROPERTY(BlueprintReadWrite)
	bool IsSit = false;
	UPROPERTY(BlueprintReadWrite)
	bool IsComment = false;

	// 인내심
	UPROPERTY(BlueprintReadOnly)
	float Patience = 100.0f;

	// 방문할 가게
	UPROPERTY(BlueprintReadOnly)
	FVector VisitDest;

	// 손님이 앉은 좌석 번호
	UPROPERTY(BlueprintReadOnly)
	int32 CurSeatNum = -1;

	FTimerHandle CustSandwichTimerHandler;

	float LeaveDelayTime;

	void MoveToDestination();

	// 손님의 취향이 아닌 재료 개수 세기
	int32 GetTotalMismatch(ASandwich* Sandwich);
	int32 CountUnpreferredIngr(TArray<int32> IngrArr);
	// 손님의 샌드위치 평점 계산하기
	void AddPlayerSandwichReview(ASandwich* Sandwich);
	// 손님의 디저트 평점 더하기
	void AddDessertReview();

	void EatSandwich();
	void ClearDestroyTimer();

	bool CanGetDessert();
	void EatDessert();

	void Eat(float EatingTime);

	float GetTip(int32 SandwichPrice);
	void AddTotalPaidPriceAndTip();

	UFUNCTION(BlueprintImplementableEvent)
	void SetReviewDialogueText(int32 TasteScore);
	UFUNCTION()
	void StartReviewDialogue(int32 TasteScore);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void TrySetComment();
	UFUNCTION(BlueprintCallable)
	FString GetComment();

	void UpdatePlayerBistroRatingSatisfaction();
	void AddPlayerBistroRatingDataInManager();

	UFUNCTION(BlueprintCallable)
	int32 GetTotalPaidPrice();
	UFUNCTION(BlueprintImplementableEvent)
	void ShowTotalPaidPrice();

	void ClearCustomerValue();
};