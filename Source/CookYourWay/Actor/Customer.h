// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Customer.generated.h"

UCLASS()
class COOKYOURWAY_API ACustomer : public ACharacter
{
	GENERATED_BODY()

	TArray<AActor*> AllCompetitorActorArr;
	class APlayerBistro* PlayerBistro;

	TMap <FVector, float> BistroLocRankMap;	// 가게 도착 위치와 계산된 점수

	float ManhattanDist(FVector Loc1, FVector Loc2);
	float CalcVisitRank(AActor* Bistro);
	void SelectBistroToVisit();

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

	UPROPERTY(BlueprintReadOnly)
	FVector VisitDest;	// 방문할 가게

	void Init();
	void SetSkeletalMesh();
};
