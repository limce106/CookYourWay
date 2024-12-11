

#include "Component/CustomerRateComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Actor/VillageManager.h"

UCustomerRateComponent::UCustomerRateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCustomerRateComponent::Init()
{
	// 손님의 평균평점, 충성도 초기화
	for (int i = 0; i < VillageManager->CustomerNames.Num(); i++) {
		CustStringToRateMap.Add(VillageManager->CustomerNames[i], 0.0f);
		CustStringToRoyaltyMap.Add(VillageManager->CustomerNames[i], 0);
	}
}

void UCustomerRateComponent::BeginPlay()
{
	Super::BeginPlay();

	VillageManager = Cast<AVillageManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AVillageManager::StaticClass()));

	Init();
}


void UCustomerRateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

