

#include "Actor/Competitor.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Component/NewsEffectComponent.h>
#include "VillageManager.h"
#include "GameInstance/CookYourWayGameInstance.h"

ACompetitor::ACompetitor()
{
	PrimaryActorTick.bCanEverTick = true;

	NewsEffectComponent = CreateDefaultSubobject<UNewsEffectComponent>(TEXT("NewsEffectComponent"));
}

void ACompetitor::BeginPlay()
{
	Super::BeginPlay();
	
	VillageManagerSystem = UCookYourWayGameInstance::GetVillageManagerSystemStatic(this);
	CustomerDataManagerSystem = UCookYourWayGameInstance::GetCustomerDataManagerSystemStatic(this);
	IngredientManagerSystem = UCookYourWayGameInstance::GetIngredientManagerSystemStatic(this);
	VillageManager = Cast<AVillageManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AVillageManager::StaticClass()));
	
	SetDefaultReviewRate();
}

TArray<int32> ACompetitor::GenerateSandwich(ACustomer* Customer)
{
	TArray<int32> SandwichIngr;

	FCustomerBistroKey Key = CustomerDataManagerSystem->GetCustomerBistroKey(Customer->CustName, AreaID);
	float Rand = FMath::RandRange(0.0f, 1.0f);

	if (Rand < 0.7f) {
		SandwichIngr = CustomerDataManagerSystem->GetCmptBestRatedCombos(Key);
		if (SandwichIngr.Num() == 0) {
			SandwichIngr = IngredientManagerSystem->GenerateRandomTaste();
		}
	}
	else {
		SandwichIngr = IngredientManagerSystem->GenerateRandomTaste();
	}

	return SandwichIngr;
}

int32 ACompetitor::GetCustomerSatisfaction(ACustomer* Customer, TArray<int32> Ingr)
{
	int32 UnPreferredIngrNum = Customer->GetUnpreferredIngrNum(Ingr);

	TArray<FCompetitorReviewData> ReviewData;
	FCompetitorData CurCompetitorData = GetCurComptitorData();
	
	if (CurCompetitorData.IsComptFestival) {	// 오픈 프로모션보다 식재료 페스티벌을 우선으로 한다.
		bool IsCustLikeIngr = IsCustLikeFestivalIngr(Customer->CustName);
	
		if (IsCustLikeIngr) {
			ReviewData = IngrFestReviewData;
		}
		else if (IsOpenPromo) {
			ReviewData = OpenPromoReviewData;
		}
		else {
			ReviewData = NormalReviewData;
		}
	}
	else if (IsOpenPromo) {
		ReviewData = OpenPromoReviewData;
	}
	else {
		ReviewData = NormalReviewData;
	}

	for (auto Data : ReviewData) {
		if (Data.UnPreferredNum == UnPreferredIngrNum) {
			return Data.Rating;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Can't Get Customer Satisfaction In Competitior!"));
	return -1;
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

bool ACompetitor::IsCustLikeFestivalIngr(FString CustName)
{
	TArray<int32> CustTaste = CustomerDataManagerSystem->CustNameToTasteMap[CustName];

	FString FestivalIngr = NewsEffectComponent->CurNewsKeyWord;
	int32 FestivalIngrIdx;
	for (int i = 0; i < IngredientManagerSystem->IngredientRows.Num(); i++) {
		if (IngredientManagerSystem->IngredientRows[i].IngrName == FestivalIngr) {
			FestivalIngrIdx = i;
			break;
		}
	}

	if (CustTaste.Contains(FestivalIngrIdx)) {
		return true;
	}
	else {
		return false;
	}
}

//int32 ACompetitor::GetCustomerSatisfaction(FString CustName)
//{
//	TArray<FCompetitorReviewData> ReviewData;
//	FCompetitorData CurCompetitorData = GetCurComptitorData();
//
//	if (CurCompetitorData.IsComptFestival) {	// 오픈 프로모션보다 식재료 페스티벌을 우선으로 한다.
//		bool IsCustLikeIngr = IsCustLikeFestivalIngr(CustName);
//
//		if (IsCustLikeIngr) {
//			ReviewData = IngrFestReviewData;
//		}
//		else if (IsOpenPromo) {
//			ReviewData = OpenPromoReviewData;
//		}
//		else {
//			ReviewData = NormalReviewData;
//		}
//	}
//	else if (IsOpenPromo) {
//		ReviewData = OpenPromoReviewData;
//	}
//	else {
//		ReviewData = NormalReviewData;
//	}
//
//	int Satisfaction = 0;
//	float Probability = FMath::FRand();
//
//	float ProbRange = 0.0f;
//
//	for (int i = 0; i < ReviewData.Num(); i++) {
//		ProbRange += ReviewData[i].RatingProb;
//		if (Probability > ProbRange) {
//			continue;
//		}
//		else {
//			Satisfaction = UKismetMathLibrary::RandomIntegerInRange(ReviewData[i].RatingMin, ReviewData[i].RatingMax);
//			break;
//		}
//	}
//
//	if (Satisfaction == 0) {
//		UE_LOG(LogTemp, Error, TEXT("Competitor: Get Customer Review Fail"));
//	}
//
//	return Satisfaction;
//}

FCompetitorData ACompetitor::GetCurComptitorData()
{
	int32 CurCompetitorDataArrIdx = VillageManagerSystem->FindCompetitorDataArrIdx(AreaID);
	FCompetitorData CompetitorData = VillageManagerSystem->CompetitorDataArr[CurCompetitorDataArrIdx];

	return CompetitorData;
}

void ACompetitor::CustomerVisited(ACustomer* Customer)
{
	VillageManager->CustomerPool->ReturnCustomer(Customer);

	GetWorld()->GetTimerManager().SetTimer(CustRatingTimerHandler, FTimerDelegate::CreateLambda([=]()
		{
			UE_LOG(LogTemp, Warning , TEXT("CustomerVisited"));
			TArray<int32> SandwichIngr = GenerateSandwich(Customer);
			int32 Satisfaction = GetCustomerSatisfaction(Customer, SandwichIngr);

			bool MaxSatisfactionUpdated = CustomerDataManagerSystem->UpdateMaxSatisfaction(Customer->CustName, AreaID, Satisfaction);
			if (MaxSatisfactionUpdated) {
				CustomerDataManagerSystem->UpdateCmptBestRatedCombos(Customer->CustName, AreaID, SandwichIngr);
			}

			float Rating = Satisfaction * 5 / 100.0f;
			// 소수점 첫째 자리까지 반올림
			float RoundRating = FMath::RoundToFloat(Rating * 10.0f) / 10.0f;

			UpdateCompetitorRating(RoundRating);
			AddCustRatingData(Customer->CustName, RoundRating);
		}), 10.0f, false);
}

void ACompetitor::UpdateCompetitorRating(float Rating)
{
	int32 Idx = VillageManagerSystem->FindCompetitorDataArrIdx(AreaID);
	VillageManagerSystem->CompetitorDataArr[Idx].TotalCust += 1;

	int32 TotalCust = VillageManagerSystem->CompetitorDataArr[Idx].TotalCust;
	float CurRatingAvg = VillageManagerSystem->CompetitorDataArr[Idx].RatingAvg;

	float UpdatedRating = ((CurRatingAvg * (TotalCust - 1)) + Rating) / (float)TotalCust;
	VillageManagerSystem->CompetitorDataArr[Idx].RatingAvg = FMath::RoundToFloat(UpdatedRating * 10.0f) / 10.0f;
}

void ACompetitor::AddCustRatingData(FString CustName, float Rating)
{
	UE_LOG(LogTemp, Warning, TEXT("AddCustRatingData"));
	FString WeekDay = VillageManager->DayToWeekString(VillageManagerSystem->Day);
	FCompetitorRatingData RatingData = FCompetitorRatingData(CustName, WeekDay, Rating);

	int32 Idx = VillageManagerSystem->FindCompetitorDataArrIdx(AreaID);
	VillageManagerSystem->CompetitorDataArr[Idx].RatingDataArr.Add(RatingData);
}
