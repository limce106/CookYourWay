// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/NewsWidget.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/IngredientManagerSystem.h"
#include "GameInstance/CustomerDataManagerSystem.h"
#include "GameInstance/VillageManagerSystem.h"
#include <Kismet/KismetMathLibrary.h>
#include "Components/RichTextBlock.h"
#include "Components/Image.h"
#include "Components/BackgroundBlur.h"
#include <GameInstance/CookYourWayGameInstance.h>

void UNewsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	VillageManagerSystem = UCookYourWayGameInstance::GetVillageManagerSystemStatic(this);
	CustomerDataManagerSystem = UCookYourWayGameInstance::GetCustomerDataManagerSystemStatic(this);
	IngredientManagerSystem = UCookYourWayGameInstance::GetIngredientManagerSystemStatic(this);

	RichTextBlock_News = (URichTextBlock*)GetWidgetFromName(TEXT("RichTextBlock_News"));
	Image_News = (UImage*)GetWidgetFromName(TEXT("Image_News"));
	BackgroundBlur_CustBlur = (UBackgroundBlur*)GetWidgetFromName(TEXT("BackgroundBlur_CustBlur"));

	FString News = GetNewsString();
	RichTextBlock_News->SetText(FText::FromString(News));

	if (VillageManagerSystem->Day == 1)
	{
		Image_News->SetBrushFromTexture(SandwichIcon);
	}
}

void UNewsWidget::NativeTick(const FGeometry& Geometry, float DeltaSeconds)
{
	Super::NativeTick(Geometry, DeltaSeconds);
}

FString UNewsWidget::RedefineNewsString(FString News, FString& OutKeyWord, FString& OutKeyWordNum)
{
	FString Redefined = News;
	FString PreNumber = "-1";

	for (int idx = 0; idx < Redefined.Len(); idx++) {
		if (Redefined[idx] == '{') {
			FString CurNumber = Redefined.Mid(idx + 1, 1);

			// 하나의 뉴스 스트링 안에서 번호가 이전에 나왔던 번호와 같은 번호이면 값이 동일하다.
			if (CurNumber != PreNumber) {
				PreNumber = CurNumber;
				// RedefineNewsString에서 키워드 종류 번호와 실제 키워드를 모두 반환합니다.
				OutKeyWordNum = CurNumber;
				OutKeyWord = GetKeyWordByNum(CurNumber);
				VillageManagerSystem->NewsKeyWord = OutKeyWord;
			}

			FString tmp1 = Redefined.Mid(0, idx);
			FString tmp2 = Redefined.Mid(idx + 3, Redefined.Len() - (idx + 3));

			Redefined = (tmp1.Append(OutKeyWord)).Append(tmp2);
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

	return OriginalNewsStr;
}

void UNewsWidget::SetNewsImage(FString Num, const FString& CurNewsKeyWord)
{
	IsCustBlur = false;
	UTexture2D* IconToSet = nullptr;

	if (Num == "1") {
		for (auto Row : CustomerDataManagerSystem->CustomerTableRows) {
			if (Row->CustName == CurNewsKeyWord) {
				IconToSet = Row->CustIcon;
				break;
			}
		}
	}
	else if (Num == "2") {
		for (auto Row : IngredientManagerSystem->IngredientTableRows) {
			if (Row->IngrName == CurNewsKeyWord) {
				IconToSet = Row->IngrIcon;
				break;
			}
		}
	}
	else if (Num == "4") {
		for (auto Row : VillageManagerSystem->StoreTableRows) {
			if (Row->StoreName == CurNewsKeyWord) {
				IconToSet = Row->StoreIcon;
				break;
			}
		}
	}
	else if (Num == "5")
	{
		int32 RandomCustIdx = UKismetMathLibrary::RandomIntegerInRange(0, CustomerDataManagerSystem->CustomerNames.Num() - 1);
		IconToSet = CustomerDataManagerSystem->CustomerTableRows[RandomCustIdx]->CustIcon;
		IsCustBlur = true;

		CustomerDataManagerSystem->SetCustTastes();
	}

	if (IconToSet)
	{
		Image_News->SetBrushFromTexture(IconToSet);
	}
}

FString UNewsWidget::GetSeasonNewsNextString()
{
	int32 TodayNewsIdx = GetYesterDayNewsIdx() + 1;
	FString OriginalNewsStr = VillageManagerSystem->NewsTableRows[TodayNewsIdx]->NewsString;
	VillageManagerSystem->NewsEffectCode = VillageManagerSystem->NewsTableRows[TodayNewsIdx]->NewsCode;

	return OriginalNewsStr;
}

FString UNewsWidget::GetNewsString()
{
	if (VillageManagerSystem->Day == 1)
	{
		return TEXT("루벤의 샌드위치 가게에 오신 것을 환영합니다!\n매일 샌드위치그램으로 마을 소식을 보실 수 있습니다.");
	}

	FString OriginalNewsStr;
	ContinueIngrSeasonDay = IsContinueIngrSeasonDay();

	if (ContinueIngrSeasonDay) {
		OriginalNewsStr = GetSeasonNewsNextString();
	}
	else {
		OriginalNewsStr = GetRandomOriginalNewsStr();
	}

	FString KeyWord;
	FString KeyWordNum;

	FString RedefinedNews = RedefineNewsString(OriginalNewsStr, KeyWord, KeyWordNum);

	if (KeyWordNum.Len() > 0)
	{
		SetNewsImage(KeyWordNum, KeyWord);
	}

	return RedefinedNews;
}

FString UNewsWidget::GetKeyWordByNum(FString Num)
{
	TArray<FString> KeyWordArr;
	if (Num == "1") {
		for (auto CustomerData : CustomerDataManagerSystem->CustomerTableRows) {
			KeyWordArr.Add(CustomerData->CustName);
		}
	}
	else if (Num == "2") {
		if (ContinueIngrSeasonDay) {
			KeyWordArr.Add(VillageManagerSystem->NewsKeyWord);
		}
		else {
			for (auto IngrData : IngredientManagerSystem->IngredientRows) {
				if (IngrData.IngrType != "Bread" || IngrData.IngrType != "Dessert") {
					KeyWordArr.Add(IngrData.IngrName);
				}
			}
		}
	}
	else if (Num == "3") {
		TArray<int32> ComptAreaIDIdxs;

		for (auto& CompetitorData : VillageManagerSystem->CompetitorDataArr) {
			KeyWordArr.Add(CompetitorData.ComptName);
			ComptAreaIDIdxs.Add(CompetitorData.AreaID);
		}

		int32 RandomIdx = UKismetMathLibrary::RandomIntegerInRange(0, KeyWordArr.Num() - 1);
		FString KeyWord = KeyWordArr[RandomIdx];
		int32 CmptDataArrIdx = VillageManagerSystem->FindCompetitorDataArrIdx(ComptAreaIDIdxs[RandomIdx]);
		VillageManagerSystem->CompetitorDataArr[CmptDataArrIdx].IsComptFestival = true;
		return KeyWord;
	}
	else if (Num == "4") {
		for (auto StoreData : VillageManagerSystem->StoreDataArr) {
			if (StoreData.StoreTableData.StorePeriod != 0) {
				KeyWordArr.Add(StoreData.StoreTableData.StoreName);
			}
		}
	}
	else if (Num == "5") {
		int32 RandomCustIdx = UKismetMathLibrary::RandomIntegerInRange(0, CustomerDataManagerSystem->CustomerNames.Num() - 1);
		FString RandomCustName = CustomerDataManagerSystem->CustomerNames[RandomCustIdx];
		TArray<int32> CustTasteArr = CustomerDataManagerSystem->CustNameToTasteMap[RandomCustName];

		for (int i = 0; i < CustTasteArr.Num(); i++) {
			KeyWordArr.Add(IngredientManagerSystem->IngredientRows[CustTasteArr[i]].IngrName);
		}
	}

	if (KeyWordArr.Num() > 0)
	{
		int32 RandomIdx = UKismetMathLibrary::RandomIntegerInRange(0, KeyWordArr.Num() - 1);
		return KeyWordArr[RandomIdx];
	}

	return FString();
}

bool UNewsWidget::IsContinueIngrSeasonDay()
{
	if (VillageManagerSystem->NewsEffectCode.Contains("IngrSeasonDay")) {
		int32 YesterDayNewsIdx = GetYesterDayNewsIdx();
		bool IsLastRow = YesterDayNewsIdx == VillageManagerSystem->NewsTableRows.Num() - 1;
		if (!IsLastRow && VillageManagerSystem->NewsTableRows[YesterDayNewsIdx + 1]->NewsCode.Contains("IngrSeasonDay")) {
			return true;
		}
	}
	return false;
}

int32 UNewsWidget::GetYesterDayNewsIdx()
{
	int32 idx = -1;
	for (int i = 0; i < VillageManagerSystem->NewsTableRows.Num(); i++) {
		if (VillageManagerSystem->NewsTableRows[i]->NewsCode == VillageManagerSystem->NewsEffectCode) {
			idx = i;
		}
	}

	return idx;
}