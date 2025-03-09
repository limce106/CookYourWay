// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ingredient.h"
#include "Sandwich.generated.h"

UCLASS()
class COOKYOURWAY_API ASandwich : public AActor
{
	GENERATED_BODY()

	class UIngredientManagerSystem* IngredientManagerSystem;
	USceneComponent* DefaultRootComponent;
	

	// 현재 제일 위에 있는 재료의 위치
	float LastIngrLocZ;
	
public:	
	ASandwich();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<AIngredient*> Ingredients;

	// 재료 추가
	UFUNCTION(BlueprintCallable)
	void AddIngredient(AIngredient* Ingr);
	// 샌드위치 제거
	UFUNCTION(BlueprintCallable)
	void DestroySandwich();
	// 샌드위치 재료들 번호 배열 (참고: 정렬X)
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetIngrNum();
	UFUNCTION(BlueprintCallable)
	bool IsMeatBurn();
	UFUNCTION(BlueprintCallable)
	void SandwichInteraction();
	UFUNCTION(BlueprintCallable)
	bool IsFirstIngrBread();
	UFUNCTION(BlueprintCallable)
	bool IsLastIngrBread();
	// 완성된 샌드위치인지(맨 처음과 마지막 재료가 빵인지)
	UFUNCTION(BlueprintCallable)
	bool IsCompleteSandwich();

	UFUNCTION(BlueprintImplementableEvent)
	void AddIngredientImg();
	UFUNCTION(BlueprintImplementableEvent)
	void SetIngrWidgetVisibility(ESlateVisibility Visibility);
};
