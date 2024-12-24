

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
	SetDefaultReviewRate();
}

void ACompetitor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACompetitor::SetDefaultReviewRate()
{
	NormalReviewData = VillageManagerSystem->GetCompetitorReviewDataOnTable("normal");
	IngrFestReviewData = VillageManagerSystem->GetCompetitorReviewDataOnTable("IngrFest");
	OpenPromoReviewData = VillageManagerSystem->GetCompetitorReviewDataOnTable("OpenPromo");
}

int ACompetitor::GetCustomerReview()
{
	int ReviewRate = 0;
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
			ReviewRate = UKismetMathLibrary::RandomIntegerInRange(NormalReviewData[i].RatingMin, NormalReviewData[i].RatingMax);
		}
	}

	if (ReviewRate == 0) {
		UE_LOG(LogTemp, Error, TEXT("Competitor: Get Customer Review Fail"));
	}

	return ReviewRate;
}

void ACompetitor::CustomerVisited(ACustomer* Customer)
{
	Customer->Destroy();
	VisitedCustNum++;

	CustomerDataManagerSystem->UpdateAvgRate(Customer->CustName, AreaID, VisitedCustNum, GetCustomerReview());
}