// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CookingUtensil.generated.h"

UCLASS()
class COOKYOURWAY_API ACookingUtensil : public AActor
{
	GENERATED_BODY()

	class AReuben* Reuben;
	
public:	
	ACookingUtensil();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// �������� ���� ��ᰡ �ö󰬴���
	UPROPERTY(BlueprintReadWrite)
	bool IsIngredientOn = false;
	// �������� ���� �ö� ��� ����
	UPROPERTY(BlueprintReadWrite)
	class AIngredient* PlacedIngredient;

	virtual float GetOneCookIncreasement();

	void PutIngrOn(AIngredient* Ingr);
	void PickUpIngr();
};
