

#include "Component/CustomerRateComponent.h"
#include <Kismet/GameplayStatics.h>

UCustomerRateComponent::UCustomerRateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCustomerRateComponent::Init()
{
	// �մ��� �������, �漺�� �ʱ�ȭ
	for (int i = 0; i < VillageManagerSystem->CustomerNames.Num(); i++) {
		CustStringToRateMap.Add(VillageManagerSystem->CustomerNames[i], 0.0f);
		CustStringToRoyaltyMap.Add(VillageManagerSystem->CustomerNames[i], 0);
	}
}

void UCustomerRateComponent::BeginPlay()
{
	Super::BeginPlay();

	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();

	Init();
}


void UCustomerRateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

