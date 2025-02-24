// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CustomerPool.h"

ACustomerPool::ACustomerPool()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACustomerPool::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACustomerPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACustomerPool::InitCustomerPool()
{
	for (int32 i = 0; i < PoolSize; i++) {
		ACustomer* NewCustomer = GetWorld()->SpawnActor<ACustomer>(BP_Customer, FVector(0.0f, 0.0f, 10.f), FRotator::ZeroRotator);
		if (NewCustomer) {
			NewCustomer->SetActorHiddenInGame(true);
			NewCustomer->SetActorEnableCollision(false);
			NewCustomer->SetActorTickEnabled(false);
			CustomerPool.Add(NewCustomer);
		}
	}
}

ACustomer* ACustomerPool::GetPooledCustomer(FString CustName, bool IsWalk)
{
	for (ACustomer* Customer : CustomerPool) {
		// 비활성화된 손님 찾기
		if (!Customer->IsActorTickEnabled()) {
			Customer->SetActorTickEnabled(true);
			Customer->SetActorHiddenInGame(false);
			Customer->SetActorEnableCollision(true);

			Customer->ClearCustomerValue();
			Customer->Init(CustName, IsWalk);

			CustomerPool.Remove(Customer);

			return Customer;
		}
	}

	ACustomer* NewCustomer = GetWorld()->SpawnActor<ACustomer>(BP_Customer);
	if (NewCustomer)
	{
		NewCustomer->SetActorTickEnabled(true);
		NewCustomer->SetActorHiddenInGame(false);
		NewCustomer->SetActorEnableCollision(true);

		NewCustomer->ClearCustomerValue();
		NewCustomer->Init(CustName, IsWalk);

		return NewCustomer;
	}

	return nullptr;
}

void ACustomerPool::ReturnCustomer(ACustomer* Customer)
{
	if (Customer) {
		Customer->SetActorTickEnabled(false);
		Customer->SetActorHiddenInGame(true);
		Customer->SetActorEnableCollision(false);

		Customer->SetActorLocation(FVector::ZeroVector);

		if (!CustomerPool.Contains(Customer))
		{
			CustomerPool.Add(Customer);
		}
	}
}

