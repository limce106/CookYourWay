// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Customer.generated.h"

UCLASS()
class COOKYOURWAY_API ACustomer : public APawn
{
	GENERATED_BODY()

	TArray<AActor*> AllCompetitorActorArr;
	class APlayerBistro* PlayerBistro;

	TMap <FVector, float> BistroLocRankMap;	// ���� ���� ��ġ�� ���� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;
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
	FString CustName;	/*�մ� ���� �� �̸� �� ���� �ʿ�*/
	UPROPERTY(BlueprintReadWrite)
	bool IsWalk = true;

	UPROPERTY(BlueprintReadOnly)
	FVector VisitDest;	// �湮�� ����

	void Init();
	void SetSkeletalMesh();
};
