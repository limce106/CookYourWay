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
	TSubclassOf<ASandwich> BP_Sandwich;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BP_Dessert;

	class APlayerBistro* PlayerBistro;
	FTimerHandle CustSandwichTimerHandler;

	// �÷��̾� �����̱�
	void MoveForward(float Value);
	void MoveRight(float Value);

	void Chop();

	// �մԿ��� ������ġ �ֱ�
	void GiveSandwich(ACustomer* Customer);
	// �մԿ��� ����Ʈ �ֱ�
	void GiveDessert(ACustomer* Customer);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 GivenSandwichNum = 0;

	UClass* GetHeldActorClass();

	// ���Ͽ� ���� ����/����
	UFUNCTION(BlueprintCallable)
	void HoldActor(AActor* Actor);
	UFUNCTION(BlueprintCallable)
	void PutDownActor();

	void SetHeldActorLoc();

	// �մԿ��� ���� �ֱ�
	void TryGiveSomething(ACustomer* Customer);
};
