// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Widget/CookRateWidget.h"
#include "CookingUtensil.generated.h"

UCLASS()
class COOKYOURWAY_API ACookingUtensil : public AActor
{
	GENERATED_BODY()
	
public:	
	ACookingUtensil();

protected:
	class AReuben* Reuben;

	virtual void BeginPlay() override;
	bool CommonCookingUtensilInteraction();

public:	

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	UCookRateWidget* BP_CookRateWidget;

	// �������� ���� ��ᰡ �ö󰬴���
	UPROPERTY(BlueprintReadWrite)
	bool IsIngredientOn = false;
	// �������� ���� �ö� ��� ����
	UPROPERTY(BlueprintReadWrite)
	class AIngredient* PlacedIngredient;

	UFUNCTION()
	virtual float GetCookIncreasement();
	UFUNCTION()
	virtual void PutIngrOn(AIngredient* Ingr);
	UFUNCTION()
	void PickUpIngr();
};
