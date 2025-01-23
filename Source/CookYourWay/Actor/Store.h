
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Customer.h"
#include "GameInstance/VillageManagerSystem.h"
#include "Store.generated.h"

UCLASS()
class COOKYOURWAY_API AStore : public AActor
{
	GENERATED_BODY()

	USceneComponent* MeshPivot;
	UStaticMeshComponent* StaticMesh;

	class UVillageManagerSystem* VillageManagerSystem;

	float TempDelayTime;
	// 스폰 주기(초)
	int32 SpawnDelayTime = 5.0f;

	void SetPivotCenter();
	void SetStoreMesh();
	bool DelayWithDeltaTime(float DelayTime, float DeltaSeconds);

	TArray<FString> GetStoreCustNames();

public:
	AStore();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACustomer> BP_Customer;

	FStoreData CurStoreData;

	virtual void Tick(float DeltaTime) override;

	void Init();
	void InitializeStoreData(const FStoreData& StoreData);
	void CreateCustomer();
};

class StoreSpawnFactory
{
public:
	static AStore* SpawnStore(UWorld* World, TSubclassOf<AStore> StoreClass, const FVector& Location, const FRotator& Rotation, FStoreData StoreData)
	{
		AStore* Store = World->SpawnActor<AStore>(StoreClass, Location, Rotation);
		Store->InitializeStoreData(StoreData);
		Store->Init();
		return Store;
	}
};