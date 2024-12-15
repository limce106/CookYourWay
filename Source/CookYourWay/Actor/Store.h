
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Customer.h"
#include "Store.generated.h"

UCLASS()
class COOKYOURWAY_API AStore : public AActor
{
	GENERATED_BODY()
	class UVillageManagerSystem* VillageManagerSystem;

	// 스폰 주기(초)
	const float SpawnDelayTime = 5.0f;

public:
	AStore();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACustomer> BP_Customer;

	virtual void Tick(float DeltaTime) override;

	void CreateCustomer();

};
