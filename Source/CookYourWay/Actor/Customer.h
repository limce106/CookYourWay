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

	TMap <FVector, float> BistroLocRankMap;	// ���� ���� ��ġ�� ���� ����

	// �Ļ� �ð��� 3�ʰ� ���� �������� ����Ʈ�� ���� �� �ִ�.
	const float CanGetDessertTime = 3.0f;
	// �ִ� ��� �ð�(��)
	const float MaxWaitingTime = 40;

	// �� ��ü ����
	int32 TotalPaidPrice = 0;

	// �մ��� �÷��̾� ���Կ� ���� ��
	FPlayerBistroRatingData PlayerBistroRatingData;

	void SetSkeletalMesh();

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

	// �Ա� ������ �ð�
	float StartEatTime = 0.0f;
	// �մ��� ����
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
	// �Դ� ������
	UPROPERTY(BlueprintReadOnly)
	bool IsEat = false;
	UPROPERTY(BlueprintReadWrite)
	bool IsWalk = false;
	UPROPERTY(BlueprintReadWrite)
	bool IsSit = false;
	UPROPERTY(BlueprintReadWrite)
	bool IsComment = false;

	// �γ���
	UPROPERTY(BlueprintReadOnly)
	float Patience = 100.0f;

	// �湮�� ����
	UPROPERTY(BlueprintReadOnly)
	FVector VisitDest;

	// �մ��� ���� �¼� ��ȣ
	UPROPERTY(BlueprintReadOnly)
	int32 CurSeatNum = -1;

	FTimerHandle CustSandwichTimerHandler;

	float LeaveDelayTime;

	void MoveToDestination();

	// �մ��� ������ �ƴ� ��� ���� ����
	int32 GetTotalMismatch(ASandwich* Sandwich);
	int32 CountUnpreferredIngr(TArray<int32> IngrArr);
	// �մ��� ������ġ ���� ����ϱ�
	void AddPlayerSandwichReview(ASandwich* Sandwich);
	// �մ��� ����Ʈ ���� ���ϱ�
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