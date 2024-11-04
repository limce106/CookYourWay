// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <GameInstance/IngredientManagerSystem.h>
#include "Ingredient.generated.h"

UCLASS()
class COOKYOURWAY_API AIngredient : public AActor
{
	GENERATED_BODY()

	class UIngredientManagerSystem* IngredientManagerSystem;

	void Init(FString IngrName);

	// 제각각인 재료들의 피봇을 맞추기 위한 함수
	void SetPivotCenter();
	void SetStaticMesh(FString IngredientName);
	
public:	
	AIngredient();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	FIngrData* CurIngrData;

	USceneComponent* MeshPivot;
	UStaticMeshComponent* StaticMesh;

	// 고기 재료 탔는지
	bool IsBurn = false;

	// 조리 정도
	UPROPERTY(BlueprintReadOnly)
	float CurCookRate = 0.0f;
	const float MaxCookRate = 1.0f;

	bool IsCooked();
};
