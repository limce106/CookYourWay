// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LevelScriptActor.h"
#include "GameInstance/VillageManagerSystem.h"
#include "GameInstance/CustomerDataManagerSystem.h"
#include "VillageLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API AVillageLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

	AVillageLevelScriptActor();

	class UVillageManagerSystem* VillageManagerSystem;
	class UCustomerDataManagerSystem* CustomerDataManagerSystem;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BP_VillageManager;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> BP_PlayerBistroHUDClass;

	UFUNCTION(BlueprintCallable)
	void Init();
};
