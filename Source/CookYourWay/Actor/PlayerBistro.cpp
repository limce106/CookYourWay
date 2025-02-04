

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
}

void APlayerBistro::SitCust(ACustomer* Customer, int32 SeatIdx)
{
	TodayCust++;
	VillageManagerSystem->PlayerBistroTotalCust++;

	IsSeated[SeatIdx] = true;
	Customer->CurSeatNum = SeatIdx;
	Customer->IsSit = true;

	Customer->SetActorLocation(CustSeatLocArr[SeatIdx]);

	ADiningTable* SeatedDiningTable = GetDiningTable(SeatIdx);
	SeatedDiningTable->SeatedCustomer = Customer;

	// 테스트
	/*TArray<int32> Arr = CustomerDataManagerSystem->GetCustTaste(Customer->CustName);
	for (int i = 0; i < Arr.Num(); i++) {
		FString name = IngredientManagerSystem->IngredientTableRowNames[Arr[i]].ToString();
		UE_LOG(LogTemp, Warning, TEXT("Taste: %s"), *name)
	}*/
	//
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
	NextCustomer->IsWaiting = false;
	NextCustomer->IncreasePatience(40);

	SitCust(NextCustomer, SeatIdx);

	NextCustomer->SetHidden(false);
	NextCustomer->SetActorEnableCollision(true);
}

void APlayerBistro::LeaveAndSitNextCust(ACustomer* LeftCustomer)
{
	if (LeftCustomer->IsEat) {
		LeftCustomer->UpdatePlayerBistroSatisfaction();
		LeftCustomer->AddPlayerBistroRatingDataInManager();
	}

	int32 LeftCustSeatIdx = LeftCustomer->CurSeatNum;
	ADiningTable* LeftCustDiningTable = GetDiningTable(LeftCustSeatIdx);
	LeftCustDiningTable->DestroyFoodOnDiningTable();
	LeftCustomer->Destroy();

	if (!WaitingCustQueue.IsEmpty()) {
		const float NextCustDelayTime = 0.5f;
		FTimerHandle DestroyCustTimerHandler;
		GetWorld()->GetTimerManager().SetTimer(DestroyCustTimerHandler, FTimerDelegate::CreateLambda([=]()
			{
				SitNextCust(LeftCustSeatIdx);
			}), NextCustDelayTime, false);
	}
	else {
		IsSeated[LeftCustSeatIdx] = false;
		GetDiningTable(LeftCustSeatIdx)->SeatedCustomer = nullptr;
	}
}

void APlayerBistro::LeaveWaitingCust(ACustomer* Customer)
{
	// 대기열에 있는 모든 손님들 중 인내심이 0이 된 손님은 맨 앞에 있는 손님이므로
	WaitingCustQueue.Pop();
	WaitingCustNum--;
	Customer->Destroy();
}