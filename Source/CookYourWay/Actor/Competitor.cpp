

#include "Actor/Competitor.h"
#include "Customer.h"

ACompetitor::ACompetitor()
{
	PrimaryActorTick.bCanEverTick = true;

	CustRateComponent = CreateDefaultSubobject<UCustomerRateComponent>(TEXT("CustRateComponent"));
}

void ACompetitor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACompetitor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACompetitor::SetCustRate(ACustomer* Customer)
{
	Customer->Destroy();
}