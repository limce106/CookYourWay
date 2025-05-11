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

	// 탄 고기 머티리얼
	UMaterialInterface* BurntMeatMaterial;

	class UIngredientManagerSystem* IngredientManagerSystem;

	// 스태틱 메시 설정 및 피봇 맞추기
	void SetMesh(FString IngrName, bool IsSliced);
	void SetScale();
	
public:	
	AIngredient();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	FIngrData CurIngrData;

	UStaticMeshComponent* OverlayMesh;
	// 고기 재료 탔는지
	bool IsBurn = false;
	// 조리 정도
	UPROPERTY(BlueprintReadOnly)
	float CurCookRate = 0.0f;
	const float MaxCookRate = 1.0f;

	// 스폰 시 필수로 호출
	UFUNCTION()
	void Init(FString IngrName, bool IsSliced);
	// 제각각인 재료들의 피봇을 맞추기 위한 함수
	UFUNCTION()
	void SetPivotCenter();
	UFUNCTION(BlueprintCallable)
	bool IsCooked();
	UFUNCTION()
	void IngredientInteraction();
	void AddBurntMaterialOverlay();
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