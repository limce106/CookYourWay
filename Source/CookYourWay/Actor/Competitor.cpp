

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
	TArray<FCompetitorReviewData> ReviewData;
	if (IsOpenPromo) {
		ReviewData = OpenPromoReviewData;
	}
	else {
		ReviewData = NormalReviewData;
	}

	int Satisfaction = 0;
	float Probability = FMath::FRand();

	float ProbRange = 0.0f;

	for (int i = 0; i < ReviewData.Num(); i++) {
		ProbRange += ReviewData[i].RatingProb;
		if (Probability > ProbRange) {
			continue;
		}
		else {
			Satisfaction = UKismetMathLibrary::RandomIntegerInRange(ReviewData[i].RatingMin, ReviewData[i].RatingMax);
			break;
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
	int32 Idx = VillageManagerSystem->FindCompetitorDataArrIdx(AreaID);
	VillageManagerSystem->CompetitorDataArr[Idx].TotalCust += 1;

	float UpdatedRating = ((VillageManagerSystem->CompetitorDataArr[Idx].Rating / 5 * 100) + GetCustomerSatisfaction()) / VillageManagerSystem->CompetitorDataArr[Idx].TotalCust;
	VillageManagerSystem->CompetitorDataArr[Idx].Rating += UpdatedRating;
}