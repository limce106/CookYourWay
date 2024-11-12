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

	class UVillageManagerSystem* VillageManagerSystem;
	class UIngredientManagerSystem* IngredientManagerSystem;

	TArray<AActor*> AllCompetitorActorArr;
	class APlayerBistro* PlayerBistro;

	TMap <FVector, float> BistroLocRankMap;	// ���� ���� ��ġ�� ���� ����

	// �մ� ���� Ÿ�̸�
	bool DestroyTimer = false;
	// �Ա� ������ �ð�
	float EatTime = 0.0f;
	// �մ��� ����
	int32 ReviewRate = 0;

	// ����ư �Ÿ� ���ϱ�
	float ManhattanDist(FVector Loc1, FVector Loc2);
	// �湮 �켱���� ���ϱ�
	float CalcVisitRank(AActor* Bistro);
	// �湮�� ������(����) ����
	void SetVisitDest();

	// ���� ����
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
	FString CustName;	/*�մ� ���� �� �̸� �� ���� �ʿ�*/
	UPROPERTY(BlueprintReadWrite)
	bool IsWalk = true;

	// �湮�� ����
	UPROPERTY(BlueprintReadOnly)
	FVector VisitDest;

	// �մ��� ���� ����
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> Taste;

	void Init();
	void SetSkeletalMesh();

	// �մ��� ������ �ƴ� ��� ���� ����
	int32 CountNotTasteNum(ASandwich* Sandwich);
	// �մ��� ������ġ ���� ����ϱ�
	void AddSandwichReview(ASandwich* Sandwich);
	// �մ��� ����Ʈ ���� ���ϱ�
	void AddDessertReview();

	void StartDestroyTimer();
	void ClearDestroyTimer();
	void DestroyCustomerAfterDelay(float DeltaTime);

	bool CanGetDessert();
};
