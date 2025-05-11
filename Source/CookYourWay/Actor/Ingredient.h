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

	// ����ƽ �޽� ���� �� �Ǻ� ���߱�
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
	// ��� ��� ������
	bool IsBurn = false;
	// ���� ����
	UPROPERTY(BlueprintReadOnly)
	float CurCookRate = 0.0f;
	const float MaxCookRate = 1.0f;

	// ���� �� �ʼ��� ȣ��
	UFUNCTION()
	void Init(FString IngrName, bool IsSliced);
	// �������� ������ �Ǻ��� ���߱� ���� �Լ�
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