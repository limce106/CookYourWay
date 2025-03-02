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

	// ź ��� ��Ƽ����
	UMaterialInterface* BurntMeatMaterial;

	class UIngredientManagerSystem* IngredientManagerSystem;

	void SetStaticMeshAndPivot(FString IngrName, bool IsSliced);
	
public:	
	AIngredient();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	FIngrData CurIngrData;

	// ��� ��� ������
	bool IsBurn = false;

	// ���� ����
	UPROPERTY(BlueprintReadOnly)
	float CurCookRate = 0.0f;
	const float MaxCookRate = 1.0f;

	// ���� �� �ʼ��� ȣ��
	void Init(FString IngrName, bool IsSliced);
	void SetScale();

	// �������� ������ �Ǻ��� ���߱� ���� �Լ�
	void SetPivotCenter();

	UFUNCTION(BlueprintCallable)
	bool IsCooked();

	void IngredientInteraction();
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