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

	// ��Ʈ�ѷ� ȸ�� ��� �� ��
	bUseControllerRotationYaw = false;
	// �̵��� �� �̵� �������� ȸ��
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
	// ���̷�Ż �޽� ����
	FString SkeletalMeshPath = FString("/Game/Assets/Art_3D/Modelling/Npc/").Append(CustName).Append("/").Append(CustName).Append(".").Append(CustName);
	USkeletalMesh* CustSkeletalMesh = LoadObject<USkeletalMesh>(NULL, *SkeletalMeshPath, NULL, LOAD_None, NULL);
	GetMesh()->SetSkeletalMesh(CustSkeletalMesh);

	// �ִϸ��̼� �������Ʈ Ŭ���� ����
	// �����Ϳ����� ����ǰ� ���� �� �� �� �� ������ �� Ȯ��!!
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

	// '������� * ����ư�Ÿ�' ���� ���� ���� ���Ը� �湮�ؾ� �ϹǷ� (�ִ� ������� - ���� �������) ���� ���ϵ��� �Ѵ�.
	// '�ִ� ������� == ���� �������'�� �� BistroRateAvg ���� 0�� �Ǿ� ManhattanDist �Լ� ���� ������� VisitRank�� 0�� �Ǵ� ���� �����ϱ� ���� 0.1�� �����־���.
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

	// �湮 �������� ������������ ����
	BistroLocRankMap.ValueSort([](const float A, const float B)
		{
			return A < B;
		});

	TArray<FVector> BistroLocRankMapKeys;
	BistroLocRankMap.GenerateKeyArray(BistroLocRankMapKeys);
	VisitDest = BistroLocRankMapKeys[0];
	return VisitDest;		// ���� ������ ���� ���Ը� �������� ����
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
	// ������ �ƴ� ��� ����
	int32 NotTasteNum = 0;

	// ù ��° ��ᰡ ���� �ƴ� ��
	if (!Sandwich->IsFirstIngrBread()) {
		NotTasteNum++;
	}
	else {
		int32 Price = IngredientManagerSystem->GetIngrSellingPrice(Sandwich->Ingredients[0]->CurIngrData.IngrClass);
		TotalPaidPrice += Price;
		Sandwich->Ingredients.RemoveAt(0);
	}

	// ������ ��ᰡ ���� �ƴ� ��
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
		// ������ġ ��ᰡ �մ��� �����̶��
		if ((Taste.Contains(IngrArr[i])) == true) {
			int32 Ingr = IngrArr[i];
			int32 Price = IngredientManagerSystem->GetSellingPriceByIndex(Ingr);
			TotalPaidPrice += Price;
			// �մ��� �� ������ ��Ḧ �� �� �̻� �������� ���ɼ��� ����Ͽ�, ���� ��ᰡ ���� �� �־ �ϳ��� ����
			Taste.RemoveSingle(Ingr);
		}
	}

	// ���Ե��� ���� ���� ������ŭ ���Ѵ�.
	UnPreferredNum += Taste.Num();
	return UnPreferredNum;
}

void ACustomer::AddPlayerSandwichReview(ASandwich* Sandwich)
{
	int TasteScore = 0;
	int NotTasteNum = GetTotalMismatch(Sandwich);
	
	// ������ ���� ��� ������ ���� ����
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

	// �γ��ɿ� ���� ���� ����
	if (Patience <= 30) {
		Satisfaction -= 10;
		PlayerBistroRatingData.IsSlowCook = true;
	}
	else if (Patience >= 70 ) {
		Satisfaction += 5;
		PlayerBistroRatingData.IsFastCook = true;
	}

	// ��Ⱑ ���ٸ� ���� ����
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
	/*�ʿ�� ������ �Դ� �ִϸ��̼� �߰�*/

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
