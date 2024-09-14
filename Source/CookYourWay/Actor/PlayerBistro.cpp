

#include "Actor/PlayerBistro.h"
#include "Customer.h"

APlayerBistro::APlayerBistro()
{
	PrimaryActorTick.bCanEverTick = true;

	CustRateComponent = CreateDefaultSubobject<UCustomerRateComponent>(TEXT("CustRateComponent"));
}

void APlayerBistro::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerBistro::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerBistro::SetCustWaitLoc(ACustomer* Customer)
{
	Customer->IsWalk = false;
	Customer->SetActorLocation(FVector(-2630.0f, -7510.0f, 0.0));	// �մ��� ���Կ� ������ ���� ��ġ

}