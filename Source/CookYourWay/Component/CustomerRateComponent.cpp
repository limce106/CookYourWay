

#include "Component/CustomerRateComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Actor/VillageManager.h"

UCustomerRateComponent::UCustomerRateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCustomerRateComponent::BeginPlay()
{
	Super::BeginPlay();

	VillageManager = Cast<AVillageManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AVillageManager::StaticClass()));

	// º’¥‘¿« ∆Ú±’∆Ú¡° √ ±‚»≠
	for (int i = 0; i < VillageManager->CustomerNames.Num(); i++) {
		CustStringToRateMap.Add(VillageManager->CustomerNames[i], 0.0f);
	}
}


void UCustomerRateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

