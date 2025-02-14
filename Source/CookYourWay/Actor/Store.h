
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Customer.h"
#include "GameInstance/VillageManagerSystem.h"
#include "CustomerPool.h"
#include "Store.generated.h"

UCLASS()
class COOKYOURWAY_API AStore : public AActor
{
	GENERATED_BODY()

	UStaticMeshComponent* StaticMesh;
	class UNewsEffectComponent* NewsEffectComponent;

	class UVillageManagerSystem* VillageManagerSystem;
	ACustomerPool* CustomerPool;

	TArray<FString> StoreCustName;

	float TempDelayTime;

	// 손님 최소 최대 스폰 주기
	int32 SpawnCustMin;
	int32 SpawnCustMax;

	// 스폰 주기(초)
	int32 SpawnDelayTime;

	void SetStoreMesh();
	bool DelayWithDeltaTime(float DelayTime, float DeltaSeconds);

	void SetStoreCustName();
	void SetSpawnCustDelayTime();

public:
	AStore();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACustomer> BP_Customer;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACustomerPool> BP_CustomerPool;

	// 부지 번호
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 AreaID;

	FStoreTable CurStoreTableData;

	virtual void Tick(float DeltaTime) override;

	void Init();
	void InitializeStoreTableData(int32 StoreAreaID, FStoreTable StoreTableData);
	void CreateCustomer();
	FString GetRandomCustName();
};

//class StoreSpawnFactory
//{
//public:
//	static AStore* SpawnStore(UWorld* World, TSubclassOf<AStore> StoreClass, const FVector& Location, const FRotator& Rotation, int32 StoreAreaID, FStoreTable StoreTableData)
//	{
//		AStore* Store = World->SpawnActor<AStore>(StoreClass, Location, Rotation);
//		Store->InitializeStoreTableData(StoreAreaID, StoreTableData);
//		Store->Init();
//		return Store;
//	}
//};