// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VillageManager.generated.h"

UCLASS()
class COOKYOURWAY_API AVillageManager : public AActor
{
	GENERATED_BODY()

	class UIngredientManagerSystem* IngredientManagerSystem;

	TMap<int32, FVector> AreaLocMap;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_PlayerBistro;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Competitor;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Store;

	void Init();

public:
	AVillageManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	TArray<FString> CustomerNames = {
		"Amy",
		"Michelle"
	};

	TMap<FString, TArray<int32>> CustNameToTasteMap;

	// ���� �մ� �̸� ��������
	FString GetRandomCustName();

	// ���� ���� �迭 ��ȯ
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetRandomTaste();
	// ��� �մ� ������ ���� ����
	UFUNCTION(BlueprintCallable)
	void SetAllCustTastes();
	// Ư�� �մ��� ���� ��������
	TArray<int32> GetCustTaste(FString CustName);
};
