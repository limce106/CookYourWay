// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Ingredient.h"
#include "Sandwich.h"
#include "Customer.generated.h"

UCLASS()
class COOKYOURWAY_API ACustomer : public ACharacter
{
	GENERATED_BODY()

	class AVillageManager* VillageManager;
	class UVillageManagerSystem* VillageManagerSystem;
	class UIngredientManagerSystem* IngredientManagerSystem;

	TArray<AActor*> AllCompetitorActorArr;
	class APlayerBistro* PlayerBistro;

	TMap <FVector, float> BistroLocRankMap;	// 가게 도착 위치와 계산된 점수

	// 손님 제거 타이머
	bool DestroyTimer = false;

	// 인내심
	float Patience = 100.0f;

	// 먹는 중인지
	bool IsEat = false;
	// 먹기 시작한 시간
	float StartEatTime = 0.0f;
	// 손님의 평점
	int32 ReviewRate = 0;

	// 식사 시간이 6초가 지난 시점부터 디저트를 받을 수 있다.
	const float CanGetDessertTime = 6.0f;

	void Init();

	// 멘해튼 거리 구하기
	float ManhattanDist(FVector Loc1, FVector Loc2);
	// 방문 우선순위 구하기
	float CalcVisitRank(AActor* Bistro);
	// 방문할 목적지(가게) 설정
	void SetVisitDest();

public:
	ACustomer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Competitor;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_PlayerBistro;

	UPROPERTY(BlueprintReadOnly)
	FString CustName;	/*손님 스폰 시 이름 값 설정 필요*/
	UPROPERTY(BlueprintReadWrite)
	bool IsWalk = true;

	// 방문할 가게
	UPROPERTY(BlueprintReadOnly)
	FVector VisitDest;

	// 손님이 앉은 좌석 번호
	UPROPERTY(BlueprintReadOnly)
	int32 CurSeatNum = -1;

	// 대기 중인지
	bool IsWaiting = false;

	float LeaveDelayTime;

	void SetSkeletalMesh();

	// 손님의 취향이 아닌 재료 개수 세기
	int32 CountNotTasteNum(ASandwich* Sandwich);
	// 손님의 샌드위치 평점 계산하기
	void AddSandwichReview(ASandwich* Sandwich);
	// 손님의 디저트 평점 더하기
	void AddDessertReview();

	void EatSandwich();
	void ClearDestroyTimer();

	bool CanGetDessert();
	void EatDessert();

	void Eat(float EatingTime);
};
