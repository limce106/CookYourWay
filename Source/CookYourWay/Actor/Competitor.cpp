

#include "Actor/Competitor.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Component/NewsEffectComponent.h>
#include "VillageManager.h"

ACompetitor::ACompetitor()
{
	PrimaryActorTick.bCanEverTick = true;

	NewsEffectComponent = CreateDefaultSubobject<UNewsEffectComponent>(TEXT("NewsEffectComponent"));
}

void ACompetitor::BeginPlay()
{
	Super::BeginPlay();
	
	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();
	CustomerDataManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UCustomerDataManagerSystem>();
	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();
	VillageManager = Cast<AVillageManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AVillageManager::StaticClass()));
	
	SetDefaultReviewRate();
}

void ACompetitor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FCompetitorReviewData> ACompetitor::GetCompetitorReviewDataOnTable(FString DataType)
{
	TArray<FCompetitorReviewData> CompetitorReviewData;

	for (int i = 0; i < VillageManagerSystem->CompetitorReviewTableRows.Num(); i++) {
		if (VillageManagerSystem->CompetitorReviewTableRows[i]->CmptState  == DataType) {
			CompetitorReviewData.Add(*VillageManagerSystem->CompetitorReviewTableRows[i]);
		}
	}

	if (CompetitorReviewData.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("Load Fail %s CompetitorReviewData"), *DataType);
		CompetitorReviewData.Add(FCompetitorReviewData::FCompetitorReviewData());
	}
	return CompetitorReviewData;
}

void ACompetitor::SetDefaultReviewRate()
{
	NormalReviewData = GetCompetitorReviewDataOnTable("normal");
	IngrFestReviewData = GetCompetitorReviewDataOnTable("IngrFest");
	OpenPromoReviewData = GetCompetitorReviewDataOnTable("OpenPromo");
}

int32 ACompetitor::GetCustomerSatisfaction()
{
	TArray<FCompetitorReviewData> ReviewData;
	if (IsOpenPromo) {
		ReviewData = OpenPromoReviewData;
	}
	else {
		FCompetitorData CurCompetitorData = GetCurComptitorData();
		if (CurCompetitorData.IsComptFestival) {
			ReviewData = IngrFestReviewData;
		}
		else {
			ReviewData = NormalReviewData;
		}
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

FCompetitorData ACompetitor::GetCurComptitorData()
{
	int32 CurCompetitorDataArrIdx = VillageManagerSystem->FindCompetitorDataArrIdx(AreaID);
	FCompetitorData CompetitorData = VillageManagerSystem->CompetitorDataArr[CurCompetitorDataArrIdx];

	return CompetitorData;
}

void ACompetitor::CustomerVisited(ACustomer* Customer)
{
	UpdateCompetitorRating();

	int32 Satisfaction = GetCustomerSatisfaction();
	CustomerDataManagerSystem->UpdateMaxSatisfaction(Customer->CustName, AreaID, Satisfaction);

	float Rating = Satisfaction * 5 / 100;
	// 소수점 첫째 자리까지 반올림
	AddRatingData(Customer->CustName, FMath::RoundToFloat(Rating * 10.0f) / 10.0f);
	Customer->Destroy();
}

void ACompetitor::UpdateCompetitorRating()
{
	int32 Idx = VillageManagerSystem->FindCompetitorDataArrIdx(AreaID);
	VillageManagerSystem->CompetitorDataArr[Idx].TotalCust += 1;

	int32 TotalCust = VillageManagerSystem->CompetitorDataArr[Idx].TotalCust;
	float CurRatingAvg = VillageManagerSystem->CompetitorDataArr[Idx].RatingAvg;
	float CurRating = GetCustomerSatisfaction() / 5 * 100;

	float UpdatedRating = ((CurRatingAvg * (TotalCust - 1)) + CurRating) / TotalCust;
	VillageManagerSystem->CompetitorDataArr[Idx].RatingAvg = FMath::RoundToFloat((UpdatedRating * 10.0f) / 10.0f);
}

void ACompetitor::AddRatingData(FString CustName, float Rating)
{
	FCompetitorData CurCompetitorData = GetCurComptitorData();

	FString WeekDay = VillageManager->DayToWeekString(VillageManagerSystem->Day);
	FCompetitorRatingData RatingData = FCompetitorRatingData(CustName, WeekDay, Rating);
	CurCompetitorData.RatingDataArr.Add(RatingData);
}
