

#include "Actor/Competitor.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <Component/NewsEffectComponent.h>

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
	
	SetDefaultReviewRate();
}

void ACompetitor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACompetitor::SetIsCmptFestival()
{
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

	int32 Satisfaction = GetCustomerSatisfaction();
	TryAddCmptFestivalSatisfaction(Customer, Satisfaction);
	CustomerDataManagerSystem->UpdateMaxSatisfaction(Customer->CustName, AreaID, Satisfaction);
	Customer->Destroy();
}

void ACompetitor::UpdateTotalCustAndRateSum()
{
	int32 Idx = VillageManagerSystem->FindCompetitorDataArrIdx(AreaID);
	VillageManagerSystem->CompetitorDataArr[Idx].TotalCust += 1;

	float UpdatedRating = ((VillageManagerSystem->CompetitorDataArr[Idx].Rating / 5 * 100) + GetCustomerSatisfaction()) / VillageManagerSystem->CompetitorDataArr[Idx].TotalCust;
	VillageManagerSystem->CompetitorDataArr[Idx].Rating += UpdatedRating;
}

void ACompetitor::TryAddCmptFestivalSatisfaction(ACustomer* Customer, int32 Satisfaction)
{
	int32 CmptDataArrIdx = VillageManagerSystem->FindCompetitorDataArrIdx(AreaID);
	if (NewsEffectComponent->CurNewsEffect == "CmptFestival" && VillageManagerSystem->CompetitorDataArr[CmptDataArrIdx].IsComptFestival == true) {
		bool IsSatisfyCustTaste = false;
		TArray<int32> CustTasteArr = CustomerDataManagerSystem->CustNameToTasteMap[Customer->CustName];
		for (int i = 0; i < CustTasteArr.Num(); i++) {
			FString IngrName = IngredientManagerSystem->IngredientRows[CustTasteArr[i]]->IngrName;
			if (NewsEffectComponent->CurNewsKeyWord == IngrName) {
				IsSatisfyCustTaste = true;
				break;
			}
		}

		if (IsSatisfyCustTaste) {
			Satisfaction += 20;
		}
	}
}
