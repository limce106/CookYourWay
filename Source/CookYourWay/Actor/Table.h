// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Table.generated.h"

UCLASS()
class COOKYOURWAY_API ATable : public AActor
{
	GENERATED_BODY()

	class AReuben* Reuben;
	
public:	
	ATable();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// ���̺� ���� ���Ͱ� �ִ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsActorOn = false;
	// ���̺� ���� �ö� ����
	UPROPERTY(BlueprintReadWrite)
	AActor* PlacedActor;

	UFUNCTION(BlueprintCallable)
	void PutActorOn(AActor* Actor);
	UFUNCTION(BlueprintCallable)
	void PickUpActor(AActor* PickUpCharacter);

	void TableInteraction();
};
