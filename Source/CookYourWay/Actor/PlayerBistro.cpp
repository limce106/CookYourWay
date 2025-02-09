

#include "Actor/PlayerBistro.h"
#include "Customer.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/VillageManagerSystem.h"
#include "Dessert.h"

APlayerBistro::APlayerBistro()
{
	PrimaryActorTick.bCanEverTick = true;

}

void APlayerBistro::SpawnDiningTable()
{
	for (int i = 0; i < CustSeatLocArr.Num(); i++) {
		FVector Location = CustSeatLocArr[i];
		Location.Y += 100.0f;
		Location.Z = 0.0f;

		GetWorld()->SpawnActor<ADiningTable>(BP_DiningTable, Location, FRotator::ZeroRotator);
	}
}

ADiningTable* APlayerBistro::GetDiningTable(int32 SeatIdx)
{
	TArray<AActor*> DiningTablesArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BP_DiningTable, DiningTablesArr);
	ADiningTable* DiningTable = Cast<ADiningTable>(DiningTablesArr[SeatIdx]);

	return DiningTable;
}

void APlayerBistro::BeginPlay()
{
	Super::BeginPlay();

	// 테스트
	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();
	//

	CustomerDataManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UCustomerDataManagerSystem>();
	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();

	SpawnDiningTable();
	IsSeated.Init(false, 5);
}

void APlayerBistro::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DelayWithDeltaTime(1.0f, DeltaTime)) {
		DecreaseWaitingCustPatience();
	}
}

void APlayerBistro::SitCust(ACustomer* Customer, int32 SeatIdx)
{
	Customer->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));

	IsSeated[SeatIdx] = true;
	Customer->CurSeatNum = SeatIdx;

	Customer->SetActorLocation(CustSeatLocArr[SeatIdx]);

	ADiningTable* SeatedDiningTable = GetDiningTable(SeatIdx);
	SeatedDiningTable->SeatedCustomer = Customer;
}

void APlayerBistro::WaitCust(ACustomer* Customer)
{
	WaitingCustQueue.Enqueue(Customer->CustName);
	WaitingCustNum++;
	WaitingCustPatience.Add(100.0);

	Customer->Destroy();
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
		if (IsSeated[i] == false) {
			return i;
		}
	}

	int NoEmptySeat = -1;
	return NoEmptySeat;
}

void APlayerBistro::CustomerVisited(ACustomer* Customer)
{
	AAIController* AINpcController = Cast<AAIController>(Customer->GetController());
	AINpcController->StopMovement();
	Customer->IsWalk = false;
	Customer->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));

	SitOrWaitCust(Customer);
}

bool APlayerBistro::DelayWithDeltaTime(float DelayTime, float DeltaSeconds)
{
	if (TempDelayTime > DelayTime) {
		TempDelayTime = 0;
		return true;
	}
	else {
		TempDelayTime += DeltaSeconds;
		return false;
	}
}

void APlayerBistro::DecreaseWaitingCustPatience()
{
	// 40초 후 인내심은 0이 된다.
	for (auto Patience : WaitingCustPatience) {
		Patience -= (100 / MaxWaitingTime);

		if (Patience <= 0) {
			LeaveWaitingCust();
		}
	}
}

int32 APlayerBistro::GetWaitingCustNum()
{
	return WaitingCustNum;
}

void APlayerBistro::SitNextCust(int32 SeatIdx)
{
	FString WaitingCustName;
	WaitingCustQueue.Dequeue(WaitingCustName);
	WaitingCustNum--;

	ACustomer* NextCustomer = CustomerSpawnFactory::SpawnCustomer(GetWorld(), BP_Customer, FVector::ZeroVector, FRotator(0.0f, 90.0f, 0.0f), WaitingCustName, false);
	NextCustomer->Patience = WaitingCustPatience[0];
	WaitingCustPatience.RemoveAt(0);
	NextCustomer->Patience += 30;

	SitCust(NextCustomer, SeatIdx);
}

void APlayerBistro::LeaveAndSitNextCust(ACustomer* LeftCustomer)
{
	LeaveCustomerInBistro(LeftCustomer);

	FTimerHandle PayPriceAnimTimerHandler;
	GetWorld()->GetTimerManager().SetTimer(PayPriceAnimTimerHandler, FTimerDelegate::CreateLambda([=]()
		{
			int32 LeftCustSeatIdx = LeftCustomer->CurSeatNum;
			ADiningTable* LeftCustDiningTable = GetDiningTable(LeftCustSeatIdx);
			LeftCustDiningTable->DestroyFoodOnDiningTable();
			LeftCustomer->Destroy();

			if (!WaitingCustQueue.IsEmpty()) {
				const float NextCustDelayTime = 0.5f;
				FTimerHandle SitNextCustTimerHandler;
				GetWorld()->GetTimerManager().SetTimer(SitNextCustTimerHandler, FTimerDelegate::CreateLambda([=]()
					{
						SitNextCust(LeftCustSeatIdx);
					}), NextCustDelayTime, false);
			}
			else {
				IsSeated[LeftCustSeatIdx] = false;
				GetDiningTable(LeftCustSeatIdx)->SeatedCustomer = nullptr;
			}
		}), 0.5f, false);
}

void APlayerBistro::LeaveWaitingCust()
{
	// 대기열에 있는 모든 손님들 중 인내심이 0이 된 손님은 맨 앞에 있는 손님이므로
	WaitingCustQueue.Pop();
	WaitingCustNum--;
	WaitingCustPatience.RemoveAt(0);
}

void APlayerBistro::LeaveCustomerInBistro(ACustomer* Customer) 
{
	TodayCust++;
	VillageManagerSystem->PlayerBistroTotalCust++;

	if (Customer->IsEat) {
		Customer->AddTotalPaidPriceAndTip();
		Customer->UpdatePlayerBistroRatingSatisfaction();
		Customer->AddPlayerBistroRatingDataInManager();
	}
}