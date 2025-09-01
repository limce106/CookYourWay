// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ingredient.h"
#include "PreviewSandwich.h"
#include "Interface/Holdable.h"
#include "Sandwich.generated.h"

UCLASS()
class COOKYOURWAY_API ASandwich : public AActor, public IHoldable
{
	GENERATED_BODY()

	class UIngredientManagerSystem* IngredientManagerSystem;
	USceneComponent* DefaultRootComponent;
	class AReuben* Reuben;
	APreviewSandwich* PreviewSandwich;

	// 현재 제일 위에 있는 재료의 위치
	float LastIngrLocZ;

	const float TableZOffset = 42.f;
	const float DiningTableZOffset = 53.f;
	
public:	
	ASandwich();

protected:
	virtual void BeginPlay() override;

public:	
	
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
	bool IsFirstIngrBread();
	UFUNCTION(BlueprintCallable)
	bool IsLastIngrBread();
	// 완성된 샌드위치인지(맨 처음과 마지막 재료가 빵인지)
	UFUNCTION(BlueprintCallable)
	bool IsCompleteSandwich();
	
	void ShowPreviewSandwich();

	UFUNCTION(BlueprintImplementableEvent)
	void AddIngredientImg();
	UFUNCTION(BlueprintImplementableEvent)
	void SetIngrWidgetVisibility(ESlateVisibility Visibility);
	UFUNCTION(BlueprintImplementableEvent)
	void SetPreviewVisibility();

	virtual void OnPutDown_Implementation(AActor* PlaceTarget) override;
	virtual void OnPickUp_Implementation() override;
};
