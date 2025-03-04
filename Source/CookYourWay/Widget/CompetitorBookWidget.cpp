// Fill out your copyright notice in the Description page of Project Settings.


#include "CompetitorBookWidget.h"
#include "Components/UniformGridPanel.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/CustomerDataManagerSystem.h"
#include "GameInstance/VillageManagerSystem.h"
#include <GameInstance/CookYourWayGameInstance.h>

void UCompetitorBookWidget::NativeConstruct()
{
	Super::NativeConstruct();

	VillageManagerSystem = UCookYourWayGameInstance::GetVillageManagerSystemStatic(this);
	CustomerDataManagerSystem = UCookYourWayGameInstance::GetCustomerDataManagerSystemStatic(this);
}

TArray<FCompetitorRatingData> UCompetitorBookWidget::GetSortedArrByGreaterRating(TArray<FCompetitorRatingData> RatingArr)
{
	TArray<FCompetitorRatingData> CopyRatingArr = RatingArr;

	CopyRatingArr.Sort([](const FCompetitorRatingData& A, const FCompetitorRatingData& B)
		{
			return A.Rating > B.Rating;
		});
	return CopyRatingArr;
}
