// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <GameInstance/IngredientManagerSystem.h>
#include "Ingredient.generated.h"

//UENUM(BlueprintType)
//enum class EIngredientState : uint8 {
//	Raw			UMETA(DisplayName = "Raw"),
//	Roasted		UMETA(DisplayName = "Roasted"),
//	Chopped		UMETA(DisplayName = "Chopped")
//};

UCLASS()
class COOKYOURWAY_API AIngredient : public AActor
{
	GENERATED_BODY()

	class UIngredientManagerSystem* IngredientManagerSystem;
	FIngrData* CurIngrData;

	void Init(FString IngrName);

	// �������� ������ �Ǻ��� ���߱� ���� �Լ�
	void SetPivotCenter();
	void SetStaticMesh(FString IngredientName);
	
public:	
	AIngredient();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	USceneComponent* MeshPivot;
	UStaticMeshComponent* StaticMesh;

	// ��� ���� �ʿ��� ���� ������ �ϼ��ߴ��� (���, ������)
	bool IsRoasted = false;
	bool IsChopped = false;

	UFUNCTION(BlueprintCallable)
	bool IsCooked();
};
