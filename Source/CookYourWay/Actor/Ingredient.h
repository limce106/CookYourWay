// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ingredient.generated.h"

UCLASS()
class COOKYOURWAY_API AIngredient : public AActor
{
	GENERATED_BODY()

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
};
