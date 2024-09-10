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

	float ManhattanDist(FVector Loc1, FVector Loc2);
	float CalcVisitRank(AActor* Bistro);
	void SelectBistroToVisit();

public:
	ACustomer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Competitor;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_PlayerBistro;

	UPROPERTY(BlueprintReadOnly)
	FString CustName;	/*�մ� ���� �� �̸� �� ���� �ʿ�*/
	UPROPERTY(BlueprintReadWrite)
	bool IsWalk = true;

	UFUNCTION(BlueprintCallable)
	void Init();
};
