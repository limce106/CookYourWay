// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CookingUtensil.h"
#include "Ingredient.h"
#include "Sandwich.h"
#include "Table.h"
#include "CuttingBoard.h"
#include "Customer.h"
#include "DiningTable.h"
#include "Reuben.generated.h"

UCLASS()
class COOKYOURWAY_API AReuben : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_PlayerBistro;
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
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Fridge;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Dessert;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACustomer> BP_Customer;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADiningTable> BP_DiningTable;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> BP_IngredientBoardClass;

	class APlayerBistro* PlayerBistro;
	FTimerHandle CustSandwichTimerHandler;

	// �÷��̾� �����̱�
	void MoveForward(float Value);
	void MoveRight(float Value);

	// ���� ��� �ִ� �Ϳ� ���� ��ȣ�ۿ�
	void EmptyOnSocketInteraction(AActor* InteractActor);
	void SandwichOnSocketInteraction(AActor* InteractActor);
	void CookingUtensilOnSocketInteraction(AActor* InteractActor);
	void IngrOnSocketInteraction(AActor* InteractActor);
	void Interaction();

	void Chop();

public:
	AReuben();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite)
	AActor* OverlappedActor;

	UPROPERTY(BlueprintReadWrite)
	bool IsHold = false;
	UPROPERTY(BlueprintReadWrite)
	AActor* HeldActor;

	// ���Ͽ� ���� ����/����
	void HoldActor(AActor* Actor);
	void PutDownActor();

	void SetHeldActorLoc();

	// �մԿ��� ���� �ֱ�
	void TryGiveSomething(ACustomer* Customer);
	// �մԿ��� ������ġ �ֱ�
	void GiveSandwich(ACustomer* Customer);
	// �մԿ��� ����Ʈ �ֱ�
	void GiveDessert(ACustomer* Customer);
};
