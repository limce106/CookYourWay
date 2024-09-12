
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Store.generated.h"

UCLASS()
class COOKYOURWAY_API AStore : public AActor
{
	GENERATED_BODY()
	
	float TempDelayTime;

public:
	AStore();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APawn> BP_Customer;

	virtual void Tick(float DeltaTime) override;

	bool DelayWithDeltaTime(float DelayTime, float DeltaSeconds);
	void CreateCustomer();

};
