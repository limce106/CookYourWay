// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CookingUtensil.h"
#include "Ingredient.h"
#include "Plate.h"
#include "Table.h"
#include "CuttingBoard.h"
#include "Reuben.generated.h"

UCLASS()
class COOKYOURWAY_API AReuben : public ACharacter
{
	GENERATED_BODY()

public:
	AReuben();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AIngredient> BP_Ingredient;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACookingUtensil> BP_CookingUtensil;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACookingUtensil> BP_CuttingBoard;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACookingUtensil> BP_FryPan;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlate> BP_Plate;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATable> BP_Table;

	UPROPERTY(BlueprintReadWrite)
	AActor* OverlappedActor;

	UPROPERTY(BlueprintReadWrite)
	bool IsHold = false;
	UPROPERTY(BlueprintReadWrite)
	AActor* FirstHoldingActor;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void EmptyOnSocketInteraction(AActor* InteractActor);
	void PlateOnSocketInteraction(AActor* InteractActor);
	void CookingUtensilOnSocketInteraction(AActor* InteractActor);
	void IngrOnSocketInteraction(AActor* InteractActor);
	void Interaction();

	void Chop();
};
