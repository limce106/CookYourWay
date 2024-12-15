

#include "Actor/PlayerBistro.h"
#include "Customer.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/VillageManagerSystem.h"

APlayerBistro::APlayerBistro()
{
	PrimaryActorTick.bCanEverTick = true;

	CustRateComponent = CreateDefaultSubobject<UCustomerRateComponent>(TEXT("CustRateComponent"));
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

	// �׽�Ʈ
	VillageManager = Cast<AVillageManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AVillageManager::StaticClass()));
	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();
	//

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
	IsSeated[SeatIdx] = true;
	Customer->CurSeatNum = SeatIdx;
	Customer->SetActorLocation(CustSeatLocArr[SeatIdx]);
	// Customer->SetActorHiddenInGame(false);

	ADiningTable* SeatedDiningTable = GetDiningTable(SeatIdx);
	SeatedDiningTable->SeatedCustomer = Customer;

	TArray<int32> Arr = VillageManager->GetCustTaste(Customer->CustName);
	// �׽�Ʈ
	for (int i = 0; i < Arr.Num(); i++) {
		FString name = IngredientManagerSystem->IngredientTableRowNames[Arr[i]].ToString();
		UE_LOG(LogTemp, Warning, TEXT("Taste: %s"), *name)
	}
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
	// ��� �մ��� �ִٸ�
	if (!WaitingCustQueue.IsEmpty()) {
		WaitCust(Customer);
	}
	else {
		int32 SeatIdx = FindEmptySeatIdx();
		// �ڸ��� �� á�ٸ� ��� �մ����� �߰�
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
	NextCustomer->IsWaiting = false;

	SitCust(NextCustomer, SeatIdx);

	NextCustomer->SetHidden(false);
	NextCustomer->SetActorEnableCollision(true);
}

void APlayerBistro::LeaveAndSitNextCust(ACustomer* LeftCustomer)
{
	int32 LeftCustSeatIdx = LeftCustomer->CurSeatNum;
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
	// ��⿭�� �ִ� ��� �մԵ� �� �γ����� 0�� �� �մ��� �� �տ� �ִ� �մ��̹Ƿ�
	WaitingCustQueue.Pop();
	WaitingCustNum--;
	Customer->Destroy();
}