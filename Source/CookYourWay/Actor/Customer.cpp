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
	PrimaryActorTick.bCanEverTick = true;

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

	Init();
}

void ACustomer::Init()
{
	CustName = CustomerDataManagerSystem->GetRandomCustName();
	SetSkeletalMesh();
	SetVisitDest();

	AAIController* AINpcController = Cast<AAIController>(GetController());
	AINpcController->MoveToLocation(VisitDest, 1.0f);
}

void ACustomer::SetSkeletalMesh()
{
	// ���̷�Ż �޽� ����
	FString SkeletalMeshPath = FString("/Game/Assets/Art_3D/Modelling/Npc/").Append(CustName).Append("/").Append(CustName).Append(".").Append(CustName);
	USkeletalMesh* CustSkeletalMesh = LoadObject<USkeletalMesh>(NULL, *SkeletalMeshPath, NULL, LOAD_None, NULL);
	GetMesh()->SetSkeletalMesh(CustSkeletalMesh);

	// �ִϸ��̼� �������Ʈ Ŭ���� ����
	// �����Ϳ����� ����ǰ� ���� �� �� �� �� ������ �� Ȯ��!!
	FString AnimBPPath = (FString("/Game/Blueprint/AnimBP/").Append(CustName).Append("_AnimBP.").Append(CustName).Append("_AnimBP"));
	UAnimBlueprint* AnimBP = LoadObject<UAnimBlueprint>(NULL, *AnimBPPath, NULL, LOAD_None, NULL);
	GetMesh()->SetAnimInstanceClass(AnimBP->GeneratedClass);
}

void ACustomer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsEat && DelayWithDeltaTime(1.0f, DeltaTime)) {
		StartEatTime += 1;
	}

	if (IsWaiting && DelayWithDeltaTime(1.0f, DeltaTime)) {
		// 40�� �� �γ����� 0�� �ȴ�.
		Patience -= (100 / MaxWaitingTime);

		if (Patience <= 0) {
			PlayerBistro->LeaveWaitingCust(this);
		}
	}

	if (!IsEat && IsSit && DelayWithDeltaTime(1.0f, DeltaTime)) {
		// ��� �ð� * 3(�ʿ� �� ���� ����)
		Patience -= (100 / MaxWaitingTime);

		if (Patience <= 0) {
			PlayerBistro->LeaveAndSitNextCust(this);
		}
	}
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
	FVector CustomerLoc = GetActorLocation();
	FVector BistroLoc = Bistro->GetActorLocation();

	int BistroAreaID = 0;
	if (Bistro->GetClass()->IsChildOf(APlayerBistro::StaticClass())) {
		BistroAreaID = PlayerBistro->AreaID;
	}
	else {
		ACompetitor* Competitor = Cast<ACompetitor>(Bistro);
		BistroAreaID = Competitor->AreaID;
	}

	// '������� * ����ư�Ÿ�' ���� ���� ���� ���Ը� �湮�ؾ� �ϹǷ� (�ִ� ������� - ���� �������) ���� ���ϵ��� �Ѵ�.
	// '�ִ� ������� == ���� �������'�� �� BistroRateAvg ���� 0�� �Ǿ� ManhattanDist �Լ� ���� ������� VisitRank�� 0�� �Ǵ� ���� �����ϱ� ���� 0.1�� �����־���.
	float BistroRateAvg = (CustomerDataManagerSystem->MaxRate + 0.1) - (CustomerDataManagerSystem->GetAvgRate(CustName, BistroAreaID));
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
		float Loyalty = *CustomerDataManagerSystem->LoyaltyMap.Find(PlayerBistroKey);

		if (Rand <= Loyalty) {
			VisitDest = *VillageManager->AreaLocMap.Find(VillageManagerSystem->PlayerBistroAreaID);
			return true;
		}
	}

	for (auto AreaID : VillageManagerSystem->CompetitorAreaID) {
		int32 Rand = UKismetMathLibrary::RandomIntegerInRange(1, 100);
		FCustomerBistroKey CompetitorKey = CustomerDataManagerSystem->GetCustomerBistroKey(CustName, AreaID);

		if (CustomerDataManagerSystem->IsRegularCustMap.Find(CompetitorKey)) {
			float Loyalty = *CustomerDataManagerSystem->LoyaltyMap.Find(CompetitorKey);

			if (Rand <= Loyalty) {
				VisitDest = *VillageManager->AreaLocMap.Find(AreaID);
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

int32 ACustomer::CountNotTasteNum(ASandwich* Sandwich)
{
	// ������ �ƴ� ��� ����
	int32 NotTasteNum = 0;
	// �� ����
	int32 BreadSellPrice = 0;

	// ù ��° ��ᰡ ���� �ƴ� ��
	if (!Sandwich->IsFirstIngrBread()) {
		NotTasteNum++;
	}
	else {
		int32 Price = IngredientManagerSystem->GetIngrSellingPrice(Sandwich->Ingredients[0]->CurIngrData->IngrClass);
		TotalSellingPrice += Price;
		Sandwich->Ingredients.RemoveAt(0);
	}

	// ������ ��ᰡ ���� �ƴ� ��
	if (!Sandwich->IsLastIngrBread()) {
		NotTasteNum++;
	}
	else {
		int32 Price = IngredientManagerSystem->GetIngrSellingPrice(Sandwich->Ingredients[Sandwich->Ingredients.Num() - 1]->CurIngrData->IngrClass);
		TotalSellingPrice += Price;
		Sandwich->Ingredients.RemoveAt(Sandwich->Ingredients.Num() - 1);
	}

	TArray<int32> IngrNumArr = Sandwich->IngrActorToNum();
	TArray<int32> Taste = CustomerDataManagerSystem->GetCustTaste(CustName);

	for (int i = 0; i < IngrNumArr.Num(); i++) {

		// ������ġ ��ᰡ �մ��� ������ �ƴ϶��
		if ((Taste.Contains(IngrNumArr[i])) == false) {
			NotTasteNum++;
		}
		// ������ġ ��ᰡ �մ��� �����̶��
		else {
			int32 Ingr = IngrNumArr[i];
			int32 Price = IngredientManagerSystem->GetSellingPriceByIndex(Ingr);
			TotalSellingPrice += Price;
			// �մ��� �� ������ ��Ḧ �� �� �̻� �������� ���ɼ��� ����Ͽ�, ���� ��ᰡ ���� �� �־ �ϳ��� ����
			Taste.RemoveSingle(Ingr);
		}
	}

	// ���Ե��� ���� ���� ������ŭ ���Ѵ�.
	NotTasteNum += Taste.Num();

	return NotTasteNum;
}

void ACustomer::AddSandwichReview(ASandwich* Sandwich)
{
	int TasteScore = 0;
	int NotTasteNum = CountNotTasteNum(Sandwich);
	
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
			TasteScore = 70;
		}
		else if (NotTasteNum == 3) {
			TasteScore = 50;
		}
		else if (NotTasteNum == 4) {
			TasteScore = 30;
		}
		else if (NotTasteNum == 5) {
			TasteScore = 10;
		}
		else {
			TasteScore = 0;
		}

		CustomerDataManagerSystem->DecreaseLoyalty(CustName, VillageManagerSystem->PlayerBistroAreaID, 10);
	}

	Satisfaction += TasteScore;

	// �γ��ɿ� ���� ���� ����
	if (Patience <= 30) {
		Satisfaction -= 10;
	}
	else if (Patience >= 70 ) {
		Satisfaction += 5;
	}

	// ��Ⱑ ���ٸ� ���� ����
	const int MeatBurnScoreDeduction = 10;
	if (Sandwich->IsMeatBurn()) {
		Satisfaction -= MeatBurnScoreDeduction;
	}

	if (Satisfaction < 0) {
		Satisfaction = 0;
	}
	else if (Satisfaction > 100) {
		Satisfaction = 100;
	}

	AddTotalSellingPriceAndTip();
}

void ACustomer::AddDessertReview()
{
	const int32 DessertBonus = 10;
	Satisfaction += DessertBonus;
}

void ACustomer::EatSandwich()
{
	// �׽�Ʈ - 7�ʷ� �����ϱ�
	Eat(7.0f);
	//

	/*�մԴ�� ��� �ʿ�*/

	//CustomerDataManagerSystem->UpdateAvgRateByCustName(CustName, PlayerBistro->AreaID, PlayerBistro->VisitedCustNum, ReviewRate);
	PlayerBistro->UpdateTotalCustAndRateSum(Satisfaction);

	// �׽�Ʈ
	UE_LOG(LogTemp, Warning, TEXT("Satisfaction: %d"), Satisfaction);
	//
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
	UE_LOG(LogTemp, Warning, TEXT("Dessert Bonus"));
}

void ACustomer::IncreasePatience(float Increasement)
{
	Patience += Increasement;
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

void ACustomer::AddTotalSellingPriceAndTip()
{
	TotalSellingPrice += GetTip(TotalSellingPrice);
	VillageManager->UpdateProfitsValue(TotalSellingPrice);
}