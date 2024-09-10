

#include "Component/CustomerRateComponent.h"

UCustomerRateComponent::UCustomerRateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCustomerRateComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCustomerRateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

