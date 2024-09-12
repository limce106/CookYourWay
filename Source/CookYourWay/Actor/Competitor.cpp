

#include "Actor/Competitor.h"
#include "Customer.h"

void ACompetitor::SetCustRate(ACustomer* Customer)
{
	Customer->Destroy();
}

ACompetitor::ACompetitor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACompetitor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACompetitor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

