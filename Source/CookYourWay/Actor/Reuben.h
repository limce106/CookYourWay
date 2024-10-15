// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CookingUtensil.h"
#include "Ingredient.h"
#include "Sandwich.h"
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
	TSubclassOf<ACookingUtensil> BP_CuttingBoard;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACookingUtensil> BP_FryPan;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Plates;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASandwich> BP_Sandwich;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATable> BP_Table;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_TrashBin;

	UPROPERTY(BlueprintReadWrite)
	AActor* OverlappedActor;

	UPROPERTY(BlueprintReadWrite)
	bool IsHold = false;

	// �÷��̾� �����̱�
	void MoveForward(float Value);
	void MoveRight(float Value);

	// ���Ͽ� ���� ����/����
	void AttachToSocket(AActor* Actor);
	void DetachActorFromSocket();

	// ���� ��� �ִ� �Ϳ� ���� ��ȣ�ۿ�
	void EmptyOnSocketInteraction(AActor* InteractActor);
	void SandwichOnSocketInteraction(AActor* InteractActor);
	void CookingUtensilOnSocketInteraction(AActor* InteractActor);
	void IngrOnSocketInteraction(AActor* InteractActor);
	void Interaction();

	void Chop();
};
