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
	class UCustomerDataManagerSystem* CustomerDataManagerSystem;

	TArray<AActor*> AllCompetitorActorArr;
	class APlayerBistro* PlayerBistro;

	TMap <FVector, float> BistroLocRankMap;	// ���� ���� ��ġ�� ���� ����

	// �ִ� ��� �ð�(��)
	const float MaxWaitingTime = 40;

	// �Ļ� �ð��� 3�ʰ� ���� �������� ����Ʈ�� ���� �� �ִ�.
	const float CanGetDessertTime = 3.0f;

	void Init();

	// ����ư �Ÿ� ���ϱ�
	float ManhattanDist(FVector Loc1, FVector Loc2);
	// �湮 �켱���� ���ϱ�
	float CalcVisitRank(AActor* Bistro);
	// �湮 �켱������ ���� ������ ����
	FVector GetDestByVisitRank();
	// �漺���� ���� ������ ����
	bool GetDestByLoyalty();
	// �湮�� ������(����) ����
	void SetVisitDest();

	float TempDelayTime;
	bool DelayWithDeltaTime(float DelayTime, float DeltaSeconds);

public:
	ACustomer();

protected:
	virtual void BeginPlay() override;

	// �մ� ���� Ÿ�̸�
	bool DestroyTimer = false;

	// �γ���
	UPROPERTY(BlueprintReadOnly)
	float Patience = 100.0f;

	// �Դ� ������
	bool IsEat = false;
	// �Ա� ������ �ð�
	float StartEatTime = 0.0f;
	// �մ��� ����
	int32 ReviewRate = 0;

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

	// �մ��� ���� �¼� ��ȣ
	UPROPERTY(BlueprintReadOnly)
	int32 CurSeatNum = -1;

	// ��� ������
	bool IsWaiting = false;
	bool IsSit = false;

	float LeaveDelayTime;

	void SetSkeletalMesh();

	// �մ��� ������ �ƴ� ��� ���� ����
	int32 CountNotTasteNum(ASandwich* Sandwich);
	// �մ��� ������ġ ���� ����ϱ�
	void AddSandwichReview(ASandwich* Sandwich);
	// �մ��� ����Ʈ ���� ���ϱ�
	void AddDessertReview();

	void EatSandwich();
	void ClearDestroyTimer();

	bool CanGetDessert();
	void EatDessert();

	void IncreasePatience(float Increasement);

	void Eat(float EatingTime);
};
