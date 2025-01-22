

#include "Actor/Competitor.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

ACompetitor::ACompetitor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACompetitor::BeginPlay()
{
	Super::BeginPlay();
	
	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();
	CustomerDataManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UCustomerDataManagerSystem>();
	
	SetDefaultReviewRate();
}

void ACompetitor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACompetitor::InitVisitNumAndSatisfationSumByCust()
{
	for (auto CustName : CustomerDataManagerSystem->CustomerNames) {
		FCustomerBistroKey Key = CustomerDataManagerSystem->GetCustomerBistroKey(CustName, AreaID);
		VisitNumByCust.Add(CustName, CustomerDataManagerSystem->VisitedNumMap[Key]);
		SatisfationSumByCust.Add(CustName, (CustomerDataManagerSystem->AvgRateMap[Key]) / 5 * 100);
	}
}

void ACompetitor::SetDefaultReviewRate()
{
	NormalReviewData = VillageManagerSystem->GetCompetitorReviewDataOnTable("normal");
	IngrFestReviewData = VillageManagerSystem->GetCompetitorReviewDataOnTable("IngrFest");
	OpenPromoReviewData = VillageManagerSystem->GetCompetitorReviewDataOnTable("OpenPromo");
}

int32 ACompetitor::GetCustomerSatisfaction()
{
	int Satisfaction = 0;
	float Probability = FMath::FRand();

	float MinProb = 0.0f;
	float MaxProb = 0.0f;

	for (int i = 0; i < NormalReviewData.Num(); i++) {
		MaxProb += NormalReviewData[i].RatingProb;
		if (Probability > MaxProb) {
			MinProb = MaxProb;
			continue;
		}
		else {
			Satisfaction = UKismetMathLibrary::RandomIntegerInRange(NormalReviewData[i].RatingMin, NormalReviewData[i].RatingMax);
		}
	}

	if (Satisfaction == 0) {
		UE_LOG(LogTemp, Error, TEXT("Competitor: Get Customer Review Fail"));
	}

	return Satisfaction;
}

void ACompetitor::CustomerVisited(ACustomer* Customer)
{
	UpdateTotalCustAndRateSum();

	VisitNumByCust.Add(Customer->CustName, VisitNumByCust[Customer->CustName] + 1);
	SatisfationSumByCust.Add(Customer->CustName, SatisfationSumByCust[Customer->CustName] + GetCustomerSatisfaction());
	Customer->Destroy();
}

void ACompetitor::UpdateTotalCustAndRateSum()
{
	int32 CurTotalCust = *VillageManagerSystem->CompetitorTotalCust.Find(AreaID);
	VillageManagerSystem->CompetitorTotalCust.Add(AreaID, ++CurTotalCust);

	int32 CurRateSum = *VillageManagerSystem->CompetitorTotalRateSum.Find(AreaID);
	VillageManagerSystem->CompetitorTotalRateSum.Add(AreaID, CurRateSum + GetCustomerSatisfaction());
}