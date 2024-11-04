// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Ingredient.h"
#include "Customer.generated.h"

UCLASS()
class COOKYOURWAY_API ACustomer : public ACharacter
{
	GENERATED_BODY()

	class UIngredientManagerSystem* IngredientManagerSystem;

	TArray<AActor*> AllCompetitorActorArr;
	class APlayerBistro* PlayerBistro;

	TMap <FVector, float> BistroLocRankMap;	// 가게 도착 위치와 계산된 점수

	// 멘해튼 거리 구하기
	float ManhattanDist(FVector Loc1, FVector Loc2);
	// 방문 우선순위 구하기
	float CalcVisitRank(AActor* Bistro);
	// 방문할 목적지(가게) 설정
	void SetVisitDest();

	// 취향 설정
	void SetTaste();

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

	// 손님의 음식 취향
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> Taste;

	void Init();
	void SetSkeletalMesh();
};
