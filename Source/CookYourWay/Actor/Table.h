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

	// 테이블 위에 액터가 있는지
	UPROPERTY(BlueprintReadWrite)
	bool IsActorOn = false;
	// 테이블 위에 올라간 액터
	UPROPERTY(BlueprintReadWrite)
	AActor* PlacedActor;

	void PutActorOn(AActor* Actor);
	void PickUpActor();

	void TableInteraction();
};
