
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

	UStaticMeshComponent* StaticMesh;

	class UVillageManagerSystem* VillageManagerSystem;

	float TempDelayTime;
	// 스폰 주기(초)
	int32 SpawnDelayTime = 5.0f;

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

	// 부지 번호
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 AreaID;

	FStoreTable CurStoreTableData;

	virtual void Tick(float DeltaTime) override;

	void Init();
	void InitializeStoreTableData(int32 StoreAreaID, FStoreTable StoreTableData);
	void CreateCustomer();
};

class StoreSpawnFactory
{
public:
	static AStore* SpawnStore(UWorld* World, TSubclassOf<AStore> StoreClass, const FVector& Location, const FRotator& Rotation, int32 StoreAreaID, FStoreTable StoreTableData)
	{
		AStore* Store = World->SpawnActor<AStore>(StoreClass, Location, Rotation);
		Store->InitializeStoreTableData(StoreAreaID, StoreTableData);
		Store->Init();
		return Store;
	}
};