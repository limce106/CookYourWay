

#include "Actor/PlayerBistro.h"
#include "Customer.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/VillageManagerSystem.h"

APlayerBistro::APlayerBistro()
{
	PrimaryActorTick.bCanEverTick = true;

	CustRateComponent = CreateDefaultSubobject<UCustomerRateComponent>(TEXT("CustRateComponent"));
}

void APlayerBistro::BeginPlay()
{
	Super::BeginPlay();
	
	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();

	IsSeated.Init(false, 5);
}

void APlayerBistro::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerBistro::SitCust(ACustomer* Customer, int32 SeatIdx)
{
	IsSeated[SeatIdx] = true;
	Customer->CurSeatNum = SeatIdx;
	Customer->SetActorLocation(CustSeatLocArr[SeatIdx]);
}

void APlayerBistro::SitOrWaitCust(ACustomer* Customer)
{
	// 대기 손님이 있다면
	if (!WaitingCustQueue.IsEmpty()) {
		WaitingCustQueue.Enqueue(Customer);
	}
	else {
		int32 SeatIdx = FindEmptySeatIdx();
		// 자리가 꽉 찼다면 대기 손님으로 추가
		if (SeatIdx == -1) {
			WaitingCustQueue.Enqueue(Customer);
			Customer->SetHidden(true);
			Customer->SetActorEnableCollision(false);
		}
		else {
			SitCust(Customer, SeatIdx);
		}
	}
}

int32 APlayerBistro::FindEmptySeatIdx()
{
	for (int i = 0; i < IsSeated.Num(); i++) {
		if (IsSeated[i] == false)
			return i;
	}

	return -1;
}

void APlayerBistro::CustomerVisited(ACustomer* Customer)
{
	VisitedCustNum++;
	Customer->IsWalk = false;

	SitOrWaitCust(Customer);
}

void APlayerBistro::UpdateCustomerReviewAvg(int32 ReveiwRate)
{
	CustomerReviewAvg = (CustomerReviewAvg * (VisitedCustNum - 1) + ReveiwRate) / VisitedCustNum;
}

void APlayerBistro::SitNextCust(int32 SeatIdx)
{
	AActor* WaitingCust;
	WaitingCustQueue.Dequeue(WaitingCust);

	WaitingCust->SetHidden(false);
	WaitingCust->SetActorEnableCollision(true);

	ACustomer* NextCustomer = Cast<ACustomer>(WaitingCust);
	SitCust(NextCustomer, SeatIdx);
}

void APlayerBistro::LeaveAndSitNextCust(ACustomer* LeftCustomer)
{
	int32 LeftCustSeatIdx = LeftCustomer->CurSeatNum;
	LeftCustomer->Destroy();

	const float NextCustDelayTime = 1.0f;
	FTimerHandle DestroyCustTimerHandler;
	GetWorld()->GetTimerManager().SetTimer(DestroyCustTimerHandler, FTimerDelegate::CreateLambda([&]()
		{
			SitNextCust(LeftCustSeatIdx);
		}), NextCustDelayTime, false);
}
