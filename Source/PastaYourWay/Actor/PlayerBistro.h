// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PastaYourWay/GameInstance/TableManagerSystem.h"

#include "PlayerBistro.generated.h"

UCLASS()
class PASTAYOURWAY_API APlayerBistro : public AActor
{
	GENERATED_BODY()

	class UTableManagerSystem* TableManagerSystem;
	
public:	
	APlayerBistro();

protected:
	virtual void BeginPlay() override;

public:	
	int32 Level;
	int32 Assets;
	float Rating;

	virtual void Tick(float DeltaTime) override;

};
