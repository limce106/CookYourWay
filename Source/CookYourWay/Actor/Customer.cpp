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

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BP_Competitor, AllCompetitorActorArr);
	PlayerBistro = Cast<APlayerBistro>(UGameplayStatics::GetActorOfClass(GetWorld(), BP_PlayerBistro));

	Init();
}

void ACustomer::Init()
{
	CustName = VillageManager->GetRandomCustName();
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

	if (IsEat && VillageManagerSystem->DelayWithDeltaTime(1.0f, DeltaTime)) {
		StartEatTime += 1;
	}


	if (IsWaiting && VillageManagerSystem->DelayWithDeltaTime(1.0f, DeltaTime)) {
		// 40�� �� �γ����� 0�� �ȴ�.
		Patience -= (100 / MaxWaitingTime);

		if (Patience <= 0) {
			PlayerBistro->LeaveWaitingCust(this);
		}
	}

	if (!IsEat && IsSit && VillageManagerSystem->DelayWithDeltaTime(3.0f, DeltaTime)) {
		// ��� �ð� * 2(�ʿ� �� ���� ����)
		Patience -= (100 / MaxWaitingTime);

		if (Patience <= 0) {
			PlayerBistro->LeaveAndSitNextCust(this);
		}
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

	UCustomerRateComponent* CustomerRateComponent = Cast<UCustomerRateComponent>(Bistro->GetComponentByClass(UCustomerRateComponent::StaticClass()));

	// '������� * ����ư�Ÿ�' ���� ���� ���� ���Ը� �湮�ؾ� �ϹǷ� (�ִ� ������� - ���� �������) ���� ���ϵ��� �Ѵ�.
	// '�ִ� ������� == ���� �������'�� �� BistroRateAvg ���� 0�� �Ǿ� ManhattanDist �Լ� ���� ������� VisitRank�� 0�� �Ǵ� ���� �����ϱ� ���� 0.1�� �����־���.
	float BistroRateAvg = (CustomerRateComponent->MaxRate + 0.1) - (*CustomerRateComponent->CustStringToRateMap.Find(CustName));
	float VisitRank = BistroRateAvg * (ManhattanDist(CustomerLoc, BistroLoc));

	BistroLocRankMap.Add(BistroLoc, VisitRank);

	return VisitRank;
}

void ACustomer::SetVisitDest()
{
	FVector CustomerLoc = GetActorLocation();

	// BistroLocRankMap.Empty();
	CalcVisitRank(PlayerBistro);

	for (auto Competitor : AllCompetitorActorArr) {
		CalcVisitRank(Competitor);
	}

	// �湮 �������� ������������ ����
	BistroLocRankMap.ValueSort([](const float A, const float B)
	{
		return A < B;
	});

	TArray<FVector> BistroLocRankMapKeys;
	BistroLocRankMap.GenerateKeyArray(BistroLocRankMapKeys);
	VisitDest = BistroLocRankMapKeys[0];	// ���� ������ ���� ���Ը� �������� ����
	VisitDest.Y += 100.0f;
	VisitDest.Z = 95.0f;
}

int32 ACustomer::CountNotTasteNum(ASandwich* Sandwich)
{
	TArray<int32> IngrNumArr = Sandwich->IngrActorToNum();
	TArray<int32> Taste = VillageManager->GetCustTaste(CustName);
	// ������ �ƴ� ��� ����
	int32 NotTasteNum = 0;

	// ù ��° ��ᰡ ���� ��
	if (IngrNumArr[0] == IngredientManagerSystem->BreadIndex) {
		IngrNumArr.RemoveAt(0);
	}
	else {
		NotTasteNum++;
	}

	// ������ ��ᰡ ���� ��
	if (IngrNumArr[IngrNumArr.Num() - 1] == IngredientManagerSystem->BreadIndex) {
		IngrNumArr.RemoveAt(IngrNumArr.Num() - 1);
	}
	else {
		NotTasteNum++;
	}

	for (int i = 0; i < IngrNumArr.Num(); i++) {

		// ������ġ ��ᰡ �մ��� ������ �ƴ϶��
		if ((Taste.Contains(IngrNumArr[i])) == false) {
			NotTasteNum++;
		}
		// ������ġ ��ᰡ �մ��� �����̶��
		else {
			// �ߺ� ������ ���� ã�� ������ ����
			int32 Ingr = IngrNumArr[i];
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
	int Score = 0;
	int NotTasteNum = CountNotTasteNum(Sandwich);
	
	// ������ ���� ��� ������ ���� ����
	if (NotTasteNum == 0) {
		Score = 100;
	}
	else if (NotTasteNum == 1) {
		Score = 90;
	}
	else if (NotTasteNum == 2) {
		Score = 70;
	}
	else if (NotTasteNum == 3) {
		Score = 50;
	}
	else if (NotTasteNum == 4) {
		Score = 30;
	}
	else if (NotTasteNum == 5) {
		Score = 10;
	}
	else {
		Score = 0;
	}

	// �γ��ɿ� ���� ���� ����
	int PatienceScoreDeduction = (100 - Patience) / 5;
	Score -= PatienceScoreDeduction;

	// ��Ⱑ ���ٸ� ���� ����
	const int MeatBurnScoreDeduction = 30;
	if (Sandwich->IsMeatBurn()) {
		Score -= MeatBurnScoreDeduction;
	}

	if (Score <= 0) {
		Score = 0;
	}

	ReviewRate += Score;
}

void ACustomer::AddDessertReview()
{
	const int32 DessertBonus = 10;
	ReviewRate += DessertBonus;
}

void ACustomer::EatSandwich()
{
	// �׽�Ʈ - 10�ʷ� �����ϱ�
	Eat(3.0f);
	//

	IncreasePatience(20.0f);
	
	/*�մԴ�� ��� �ʿ�*/

	PlayerBistro->UpdateCustomerReviewAvg(ReviewRate);

	// �׽�Ʈ
	UE_LOG(LogTemp, Warning, TEXT("ReviewRate: %d"), ReviewRate);
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
	IncreasePatience(10.0f);
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
