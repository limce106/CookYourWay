

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

void APlayerBistro::WaitCust(ACustomer* Customer)
{
	WaitingCustQueue.Enqueue(Customer);
	WaitingCustNum++;

	Customer->SetHidden(true);
	Customer->SetActorEnableCollision(false);
	Customer->IsWaiting = true;
}

void APlayerBistro::SitOrWaitCust(ACustomer* Customer)
{
	// 대기 손님이 있다면
	if (!WaitingCustQueue.IsEmpty()) {
		WaitCust(Customer);
	}
	else {
		int32 SeatIdx = FindEmptySeatIdx();
		// 자리가 꽉 찼다면 대기 손님으로 추가
		if (SeatIdx == -1) {
			WaitCust(Customer);
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

	AAIController* AINpcController = Cast<AAIController>(Customer->GetController());
	AINpcController->StopMovement();
	Customer->IsWalk = false;
	Customer->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));

	VisitedCustNum++;

	SitOrWaitCust(Customer);
}

void APlayerBistro::UpdateCustomerReviewAvg(int32 ReveiwRate)
{
	CustomerReviewAvg = (CustomerReviewAvg * (VisitedCustNum - 1) + ReveiwRate) / VisitedCustNum;
}

int32 APlayerBistro::GetWaitingCustNum()
{
	return WaitingCustNum;
}

void APlayerBistro::SitNextCust(int32 SeatIdx)
{
	AActor* WaitingCust;
	WaitingCustQueue.Dequeue(WaitingCust);
	WaitingCustNum--;

	ACustomer* NextCustomer = Cast<ACustomer>(WaitingCust);
	NextCustomer->SetHidden(false);
	NextCustomer->SetActorEnableCollision(true);
	NextCustomer->IsWaiting = false;

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

void APlayerBistro::LeaveWaitingCust(ACustomer* Customer)
{
	// 대기열에 있는 모든 손님들 중 인내심이 0이 된 손님은 맨 앞에 있는 손님이므로
	WaitingCustQueue.Pop();
	WaitingCustNum--;
	Customer->Destroy();
}
