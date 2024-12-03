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

	USceneComponent* MeshPivot;
	UStaticMeshComponent* StaticMesh;

	class UIngredientManagerSystem* IngredientManagerSystem;

	// 제각각인 재료들의 피봇을 맞추기 위한 함수
	void SetPivotCenter();
	void SetStaticMeshAndPivot(FString IngrName, bool IsSliced);
	
public:	
	AIngredient();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	FIngrData* CurIngrData;

	// 고기 재료 탔는지
	bool IsBurn = false;

	// 조리 정도
	UPROPERTY(BlueprintReadOnly)
	float CurCookRate = 0.0f;
	const float MaxCookRate = 1.0f;

	// 스폰 시 필수로 호출
	void Init(FString IngrName, bool IsSliced);

	bool IsCooked();
};

class IngredientSpawnFactory
{
public:
	static AIngredient* SpawnIngredient(UWorld* World, TSubclassOf<AIngredient> IngredientClass, const FVector& Location, const FRotator& Rotation, FString IngrName, bool IsSliced)
	{
		AIngredient* Ingredient = World->SpawnActor<AIngredient>(IngredientClass, Location, Rotation);
		Ingredient->Init(IngrName, IsSliced);
		return Ingredient;
	}
};