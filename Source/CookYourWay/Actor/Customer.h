// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerBistro.h"
#include "Customer.generated.h"

UCLASS()
class COOKYOURWAY_API ACustomer : public APawn
{
	GENERATED_BODY()

	TArray<AActor*> AllCompetitorActorArr;
	APlayerBistro* PlayerBistro;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* SkeletalMesh;

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

	void Init();
	void SetSkeletalMesh();
};
