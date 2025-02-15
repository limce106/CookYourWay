// Fill out your copyright notice in the Description page of Project Settings.


#include "Customer.h"
#include <Kismet/GameplayStatics.h>
#include "Competitor.h"
#include "PlayerBistro.h"
#include"Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "VillageManager.h"

ACustomer::ACustomer()
{
	PrimaryActorTick.bCanEverTick = false;

	// 컨트롤러 회전 사용 안 함
	bUseControllerRotationYaw = false;
	// 이동할 때 이동 방향으로 회전
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACustomer::BeginPlay()
{
	Super::BeginPlay();

	VillageManager = Cast<AVillageManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AVillageManager::StaticClass()));
	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();
	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();
	CustomerDataManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UCustomerDataManagerSystem>();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BP_Competitor, AllCompetitorActorArr);
	PlayerBistro = Cast<APlayerBistro>(UGameplayStatics::GetActorOfClass(GetWorld(), BP_PlayerBistro));
}

void ACustomer::SetSkeletalMesh()
{
	// 스켈레탈 메시 적용
	FString SkeletalMeshPath = FString("/Game/Assets/Art_3D/Modelling/Npc/").Append(CustName).Append("/").Append(CustName).Append(".").Append(CustName);
	USkeletalMesh* CustSkeletalMesh = LoadObject<USkeletalMesh>(NULL, *SkeletalMeshPath, NULL, LOAD_None, NULL);
	GetMesh()->SetSkeletalMesh(CustSkeletalMesh);

	// 애니메이션 블루프린트 클래스 적용
	// 에디터에서만 적용되고 빌드 시 안 될 수 있으니 꼭 확인!!
	FString AnimBPPath = (FString("/Game/Blueprint/AnimBP/").Append(CustName).Append("_AnimBP.").Append(CustName).Append("_AnimBP_C"));
	// UAnimBlueprint* AnimBP = LoadObject<UAnimBlueprint>(NULL, *AnimBPPath, NULL, LOAD_None, NULL);
	UClass* AnimBPClass = LoadClass<UAnimInstance>(NULL, *AnimBPPath, NULL, LOAD_None, NULL);
	if (AnimBPClass) {
		GetMesh()->SetAnimInstanceClass(AnimBPClass);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to load AnimBP for %s"), *CustName);
	}
}

void ACustomer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsEat && DelayWithDeltaTime(1.0f, DeltaTime)) {
		StartEatTime += 1;

		if (StartEatTime == CanGetDessertTime) {
			ADiningTable* DiningTable = PlayerBistro->GetDiningTable(CurSeatNum);
			DiningTable->DestroyFoodOnDiningTable();
		}
	}

	if (!IsWalk && !IsEat && DelayWithDeltaTime(1.0f, DeltaTime)) {
		Patience -= (100 / MaxWaitingTime);

		if (Patience <= 0) {
			PlayerBistro->LeaveAndSitNextCust(this);
		}
	}
}

void ACustomer::Init(FString Name, bool bWalk)
{
	this->CustName = Name;
	this->IsWalk = bWalk;

	SetSkeletalMesh();
}

bool ACustomer::DelayWithDeltaTime(float DelayTime, float DeltaSeconds)
{
	if (TempDelayTime > DelayTime) {
		TempDelayTime = 0;
		return true;
	}
	else {
		TempDelayTime += DeltaSeconds;
		return false;
	}
}

float ACustomer::ManhattanDist(FVector Loc1, FVector Loc2)
{
	float absX = abs(Loc1.X - Loc2.X);
	float absY = abs(Loc1.Y - Loc2.Y);
	return (absX + absY) / 100;
}

float ACustomer::CalcVisitRank(AActor* Bistro)
{
	float AvgRate;
	FVector CustomerLoc = GetActorLocation();
	FVector BistroLoc = Bistro->GetActorLocation();

	int BistroAreaID = 0;
	if (Bistro->GetClass()->IsChildOf(APlayerBistro::StaticClass())) {
		BistroAreaID = PlayerBistro->AreaID;
		AvgRate = VillageManagerSystem->PlayerBistroRating;
	}
	else {
		ACompetitor* Competitor = Cast<ACompetitor>(Bistro);
		BistroAreaID = Competitor->AreaID;
		int32 CompetitorDataArrIdx = VillageManagerSystem->FindCompetitorDataArrIdx(Competitor->AreaID);
		AvgRate = VillageManagerSystem->CompetitorDataArr[CompetitorDataArrIdx].RatingAvg;
	}

	// '평점평균 * 맨해튼거리' 값이 가장 작은 가게를 방문해야 하므로 (최대 평점평균 - 실제 평점평균) 값을 곱하도록 한다.
	// '최대 평점평균 == 실제 평점평균'일 때 BistroRateAvg 값이 0이 되어 ManhattanDist 함수 값에 상관없이 VisitRank이 0이 되는 것을 방지하기 위해 0.1을 더해주었다.
	float BistroRateAvg = (CustomerDataManagerSystem->MaxRate + 0.1) - AvgRate;
	float VisitRank = BistroRateAvg * (ManhattanDist(CustomerLoc, BistroLoc));

	BistroLocRankMap.Add(BistroLoc, VisitRank);

	return VisitRank;
}

FVector ACustomer::GetDestByVisitRank()
{
	FVector CustomerLoc = GetActorLocation();

	// BistroLocRankMap.Empty();
	CalcVisitRank(PlayerBistro);

	for (auto Competitor : AllCompetitorActorArr) {
		Competitor = Cast<ACompetitor>(Competitor);
		CalcVisitRank(Competitor);
	}

	// 방문 점수들을 오름차순으로 정렬
	BistroLocRankMap.ValueSort([](const float A, const float B)
		{
			return A < B;
		});

	TArray<FVector> BistroLocRankMapKeys;
	BistroLocRankMap.GenerateKeyArray(BistroLocRankMapKeys);
	VisitDest = BistroLocRankMapKeys[0];
	return VisitDest;		// 가장 점수가 낮은 가게를 목적지로 설정
}

bool ACustomer::GetDestByLoyalty()
{
	FCustomerBistroKey PlayerBistroKey = CustomerDataManagerSystem->GetCustomerBistroKey(CustName, VillageManagerSystem->PlayerBistroAreaID);
	if (CustomerDataManagerSystem->IsRegularCustMap.Find(PlayerBistroKey)) {
		int32 Rand = UKismetMathLibrary::RandomIntegerInRange(1, 100);
		int32 Loyalty = *CustomerDataManagerSystem->LoyaltyMap.Find(PlayerBistroKey);

		if (Rand <= Loyalty) {
			VisitDest = *VillageManager->AreaLocMap.Find(VillageManagerSystem->PlayerBistroAreaID);
			return true;
		}
	}

	for (FCompetitorData CompetitorData : VillageManagerSystem->CompetitorDataArr) {
		int32 Rand = UKismetMathLibrary::RandomIntegerInRange(1, 100);
		FCustomerBistroKey CompetitorKey = CustomerDataManagerSystem->GetCustomerBistroKey(CustName, CompetitorData.AreaID);

		if (CustomerDataManagerSystem->IsRegularCustMap.Find(CompetitorKey)) {
			int32 Loyalty = *CustomerDataManagerSystem->LoyaltyMap.Find(CompetitorKey);

			if (Rand <= Loyalty) {
				VisitDest = *VillageManager->AreaLocMap.Find(CompetitorData.AreaID);
				return true;
			}
		}
	}
	
	return false;
}

void ACustomer::SetVisitDest()
{
	bool SuccessFindingDest = GetDestByLoyalty();
	if (!SuccessFindingDest) {
		GetDestByVisitRank();
	}

	VisitDest.Y += 100.0f;
	VisitDest.Z = 95.0f;
}

void ACustomer::MoveToDestination()
{
	SetVisitDest();

	AAIController* AINpcController = Cast<AAIController>(GetController());
	AINpcController->MoveToLocation(VisitDest, 1.0f);
}

int32 ACustomer::GetTotalMismatch(ASandwich* Sandwich)
{
	// 취향이 아닌 재료 개수
	int32 NotTasteNum = 0;

	// 첫 번째 재료가 빵이 아닐 때
	if (!Sandwich->IsFirstIngrBread()) {
		NotTasteNum++;
	}
	else {
		int32 Price = IngredientManagerSystem->GetIngrSellingPrice(Sandwich->Ingredients[0]->CurIngrData.IngrClass);
		TotalPaidPrice += Price;
		Sandwich->Ingredients.RemoveAt(0);
	}

	// 마지막 재료가 빵이 아닐 때
	if (!Sandwich->IsLastIngrBread()) {
		NotTasteNum++;
	}
	else {
		int32 Price = IngredientManagerSystem->GetIngrSellingPrice(Sandwich->Ingredients[Sandwich->Ingredients.Num() - 1]->CurIngrData.IngrClass);
		TotalPaidPrice += Price;
		Sandwich->Ingredients.RemoveAt(Sandwich->Ingredients.Num() - 1);
	}

	TArray<int32> IngrNumArr = Sandwich->IngrActorToNum();
	PlayerBistroRatingData.GivenIngr = IngrNumArr;
	NotTasteNum += CountUnpreferredIngr(IngrNumArr);

	return NotTasteNum;
}

int32 ACustomer::CountUnpreferredIngr(TArray<int32> IngrArr)
{
	int32 UnPreferredNum = 0;

	TArray<int32> Taste = CustomerDataManagerSystem->GetCustTaste(CustName);

	for (int i = 0; i < IngrArr.Num(); i++) {
		// 샌드위치 재료가 손님의 취향이라면
		if ((Taste.Contains(IngrArr[i])) == true) {
			int32 Ingr = IngrArr[i];
			int32 Price = IngredientManagerSystem->GetSellingPriceByIndex(Ingr);
			TotalPaidPrice += Price;
			// 손님이 한 종류의 재료를 두 개 이상 선택했을 가능성을 고려하여, 같은 재료가 여러 개 있어도 하나만 삭제
			Taste.RemoveSingle(Ingr);
		}
	}

	// 포함되지 않은 취향 개수만큼 더한다.
	UnPreferredNum += Taste.Num();
	return UnPreferredNum;
}

void ACustomer::AddPlayerSandwichReview(ASandwich* Sandwich)
{
	int TasteScore = 0;
	int NotTasteNum = GetTotalMismatch(Sandwich);
	
	// 맞추지 못한 재료 개수에 따른 점수
	if (NotTasteNum == 0) {
		TasteScore = 100;
		CustomerDataManagerSystem->AddRegularCust(CustName, VillageManagerSystem->PlayerBistroAreaID);
	}
	else {
		if (NotTasteNum == 1) {
			TasteScore = 90;
		}
		else if (NotTasteNum == 2) {
			TasteScore = 75;
		}
		else if (NotTasteNum == 3) {
			TasteScore = 50;
		}
		else if (NotTasteNum == 4) {
			TasteScore = 25;
		}
		else if (NotTasteNum == 5) {
			TasteScore = 10;
		}
		else {
			TasteScore = 0;
		}

		StartReviewDialogue(TasteScore);

		CustomerDataManagerSystem->DecreaseLoyalty(CustName, VillageManagerSystem->PlayerBistroAreaID, 10);
	}

	Satisfaction += TasteScore;

	// 인내심에 따라 점수 증감
	if (Patience <= 30) {
		Satisfaction -= 10;
		PlayerBistroRatingData.IsSlowCook = true;
	}
	else if (Patience >= 70 ) {
		Satisfaction += 5;
		PlayerBistroRatingData.IsFastCook = true;
	}

	// 고기가 탔다면 점수 감소
	const int MeatBurnScoreDeduction = 10;
	if (Sandwich->IsMeatBurn()) {
		Satisfaction -= MeatBurnScoreDeduction;
	}

	Satisfaction = FMath::Clamp(Satisfaction, 0, 100);
}

void ACustomer::AddDessertReview()
{
	const int32 DessertBonus = 10;
	Satisfaction += DessertBonus;
}

void ACustomer::EatSandwich()
{
	Eat(7.0f);
}

void ACustomer::ClearDestroyTimer()
{
	DestroyTimer = false;
	StartEatTime = 0.0f;
}

bool ACustomer::CanGetDessert()
{
	if (StartEatTime >= CanGetDessertTime) {
		return true;
	}
	else {
		return false;
	}
}

void ACustomer::EatDessert()
{
	ClearDestroyTimer();
	Eat(2.0f);

	Satisfaction += 10;
	PlayerBistroRatingData.IsEatDessert = true;

	UE_LOG(LogTemp, Warning, TEXT("Dessert Bonus"));
}

void ACustomer::Eat(float EatingTime)
{
	/*필요시 음식을 먹는 애니메이션 추가*/

	IsEat = true;
	DestroyTimer = true;
	LeaveDelayTime = EatingTime;
}

float ACustomer::GetTip(int32 SandwichPrice)
{
	bool IsRegularCustomer = CustomerDataManagerSystem->IsRegularCust(CustName, VillageManagerSystem->PlayerBistroAreaID);
	if (IsRegularCustomer) {
		return SandwichPrice;
	}
	else {
		if (Satisfaction >= 90) {
			return (SandwichPrice * 0.3);
		}
		else if (Satisfaction >= 80) {
			return (SandwichPrice * 0.2);
		}
		else {
			return (SandwichPrice * 0.1);
		}
	}
}

void ACustomer::AddTotalPaidPriceAndTip()
{
	TotalPaidPrice += GetTip(TotalPaidPrice);
	VillageManager->UpdateProfitsValue(TotalPaidPrice);
	PlayerBistroRatingData.Price = TotalPaidPrice;

	ShowTotalPaidPrice();
}

void ACustomer::StartReviewDialogue(int32 TasteScore)
{
	FString ReviewDialogue = CustomerDataManagerSystem->GetCustReviewDialogue(CustName, TasteScore);
	SetReviewDialogueText(TasteScore);
	PlayerBistroRatingData.Review = ReviewDialogue;
}

FString ACustomer::GetComment()
{
	PlayerBistroRatingData.IsTalk = true;

	int32 CommentType = UKismetMathLibrary::RandomIntegerInRange(1, 2);
	int32 index = 0;

	for (auto CustCommentData : VillageManager->RedefinedCustomerCommentTableRows) {
		if (CommentType == 1 && CustCommentData.CustCode == CustName && CustCommentData.CustCommentType == CommentType) {
			Satisfaction += 20;
			VillageManager->IsCommentTalked[index] = true;
			return CustCommentData.CustCommentString;
		}
		else if (CommentType == 2 && CustCommentData.CustCode == CustName && CustCommentData.CustCommentType == CommentType) {
			VillageManager->IsCommentTalked[index] = true;
			return CustCommentData.CustCommentString;
		}
		index++;
	}

	UE_LOG(LogTemp, Error, TEXT("Can't Get Comment!"));
	return FString();
}

void ACustomer::UpdatePlayerBistroRatingSatisfaction()
{
	PlayerBistroRatingData.CustName = CustName;
	PlayerBistroRatingData.WeekDay = VillageManager->DayToWeekString(VillageManagerSystem->Day);

	float Rating = Satisfaction * 5 / 100.0f;
	Rating = FMath::RoundToFloat(Rating * 10.0f) / 10.0f;
	PlayerBistroRatingData.Rating = Rating;

	VillageManagerSystem->UpdatePlayerBistroRating(Rating);
	CustomerDataManagerSystem->UpdateMaxSatisfaction(CustName, PlayerBistro->AreaID, Satisfaction);
}

void ACustomer::AddPlayerBistroRatingDataInManager()
{
	CustomerDataManagerSystem->PlayerBistroRatingDataArr.Add(PlayerBistroRatingData);
}

int32 ACustomer::GetTotalPaidPrice()
{
	return TotalPaidPrice;
}

void ACustomer::ClearCustomerValue()
{
	TotalPaidPrice = 0;
	StartEatTime = 0.0f;
	Satisfaction = 0;

	IsEat = false;
	IsWalk = false;
	IsComment = false;

	Patience = 100.0f;

	GetMesh()->SetSkeletalMesh(nullptr);
	GetMesh()->SetAnimInstanceClass(nullptr);
}
