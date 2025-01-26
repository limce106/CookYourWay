// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NewsWidget.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/IngredientManagerSystem.h"
#include "GameInstance/CustomerDataManagerSystem.h"
#include "GameInstance/VillageManagerSystem.h"
#include <Kismet/KismetMathLibrary.h>
#include "Components/RichTextBlock.h"

void UNewsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();
	CustomerDataManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UCustomerDataManagerSystem>();
	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();

	RichTextBlock_News = (URichTextBlock*)GetWidgetFromName(TEXT("RichTextBlock_News"));
	FString News = GetRedefinedNewsString();
	RichTextBlock_News->SetText(FText::FromString(News));
}

void UNewsWidget::NativeTick(const FGeometry& Geometry, float DeltaSeconds)
{
	Super::NativeTick(Geometry, DeltaSeconds);
}

FString UNewsWidget::RedefineNewsString(FString News)
{
	FString Redefined = News;
	FString tmp1;
	FString tmp2;

	for (int idx = 0; idx < Redefined.Len(); idx++) {
		if (Redefined[idx] == '{') {
			FString Number = Redefined.Mid(idx + 1, 1);
			VillageManagerSystem->NewsKeyWord = GetKeyWordByNum(FCString::Atoi(*Number));

			tmp1 = Redefined.Mid(0, idx);
			tmp2 = Redefined.Mid(idx + 3, Redefined.Len() - (idx + 3));

			Redefined = (tmp1.Append(VillageManagerSystem->NewsKeyWord)).Append(tmp2);
		}
	}

	return Redefined;
}

FString UNewsWidget::GetRandomOriginalNewsStr()
{
	float Probability = FMath::FRand();
	float ProbRange = 0.0f;
	FString OriginalNewsStr;

	for (int i = 0; i < VillageManagerSystem->NewsTableRows.Num(); i++) {
		if (VillageManagerSystem->NewsTableRows[i]->NewsProb < 0) {
			continue;
		}
		else {
			ProbRange += VillageManagerSystem->NewsTableRows[i]->NewsProb;
		}
		
		if (Probability > ProbRange) {
			continue;
		}
		else {
			OriginalNewsStr = VillageManagerSystem->NewsTableRows[i]->NewsString;
			VillageManagerSystem->NewsEffectCode = VillageManagerSystem->NewsTableRows[i]->NewsCode;
			break;
		}
	}

	// 테스트
	OriginalNewsStr = VillageManagerSystem->NewsTableRows[7]->NewsString;
	//

	return OriginalNewsStr;
}

FString UNewsWidget::GetRedefinedNewsString()
{
	FString RedefinedNews = RedefineNewsString(GetRandomOriginalNewsStr());
	return RedefinedNews;
}

FString UNewsWidget::GetKeyWordByNum(int32 Num)
{
	TArray<FString> KeyWordArr;
	if (Num == 1) {
		for (auto CustomerData : CustomerDataManagerSystem->CustomerTableRows) {
			KeyWordArr.Add(CustomerData->CustName);
		}
	}
	else if (Num == 2) {
		for (auto IngrData : IngredientManagerSystem->IngredientRows) {
			KeyWordArr.Add(IngrData->IngrName);
		}
	}
	else if (Num == 3) {
		for (auto& CompetitorData : VillageManagerSystem->CompetitorDataArr) {
			if (!(VillageManagerSystem->IsMonday() && !CustomerDataManagerSystem->HasRegularCust(CompetitorData.AreaID))) {
				KeyWordArr.Add(FString(TEXT("%d구역 경쟁사"), CompetitorData.AreaID));
			}
		}

		int32 RandomIdx = UKismetMathLibrary::RandomIntegerInRange(0, KeyWordArr.Num() - 1);
		FString KeyWord = KeyWordArr[RandomIdx];
		int32 CmptDataArrIdx = VillageManagerSystem->FindCompetitorDataArrIdx(FCString::Atoi(*KeyWord));
		VillageManagerSystem->CompetitorDataArr[CmptDataArrIdx].IsComptFestival = true;
		return KeyWord;
	}
	else if (Num == 4) {
		for (auto StoreData : VillageManagerSystem->StoreDataArr) {
			if (StoreData.StoreTableData.StorePeriod != 1) {
				KeyWordArr.Add(StoreData.StoreTableData.StoreName);
			}
		}
	}
	else if (Num == 5) {
		int32 RandomCustIdx = UKismetMathLibrary::RandomIntegerInRange(0, CustomerDataManagerSystem->CustomerNames.Num() - 1);
		TArray<int32> CustTasteArr = CustomerDataManagerSystem->CustNameToTasteMap[CustomerDataManagerSystem->CustomerNames[RandomCustIdx]];
		
		for (int i = 0; i < CustTasteArr.Num(); i++) {
			KeyWordArr.Add(IngredientManagerSystem->IngredientRows[CustTasteArr[i]]->IngrName);
		}
	}

	int32 RandomIdx = UKismetMathLibrary::RandomIntegerInRange(0, KeyWordArr.Num() - 1);
	return KeyWordArr[RandomIdx];
}
