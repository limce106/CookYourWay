

#include "Actor/PlayerBistro.h"
#include "Customer.h"

void APlayerBistro::SetCustWaitLoc(ACustomer* Customer)
{
	Customer->SetActorLocation(FVector(-2630.0f, -7510.0f, 0.0));	// �մ��� ���Կ� ������ ���� ��ġ

}

APlayerBistro::APlayerBistro()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APlayerBistro::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerBistro::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

