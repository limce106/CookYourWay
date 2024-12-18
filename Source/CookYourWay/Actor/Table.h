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
	UPROPERTY(BlueprintReadWrite)
	bool IsActorOn = false;
	// ���̺� ���� �ö� ����
	UPROPERTY(BlueprintReadWrite)
	AActor* PlacedActor;

	void PutActorOn(AActor* Actor);
	void PickUpActor();

	void TableInteraction();
};
