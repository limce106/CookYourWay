// Fill out your copyright notice in the Description page of Project Settings.


#include "Customer.h"
#include <Kismet/GameplayStatics.h>
#include "Competitor.h"
#include "PlayerBistro.h"
#include"Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>

ACustomer::ACustomer()
{
	PrimaryActorTick.bCanEverTick = true;

	// ��Ʈ�ѷ� ȸ�� ��� �� ��
	bUseControllerRotationYaw = false;
	// �̵��� �� �̵� �������� ȸ��
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACustomer::Init()
{
	CustName = "Michelle"; // ���Ƿ� �׽�Ʈ�� ���� ����, 
	SetSkeletalMesh();
	SetVisitDest();

	AAIController* AINpcController = Cast<AAIController>(GetController());
	AINpcController->MoveToLocation(VisitDest, 1.0f);

	SetTaste();
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

void ACustomer::BeginPlay()
{
	Super::BeginPlay();
	
	VillageManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UVillageManagerSystem>();
	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BP_Competitor, AllCompetitorActorArr);
	PlayerBistro = Cast<APlayerBistro>(UGameplayStatics::GetActorOfClass(GetWorld(), BP_PlayerBistro));

	Init();
}

void ACustomer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsEat) {
		StartEatTime += DeltaTime;
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
	ECustType CurCustType = *CustomerRateComponent->CustStringToTypeMap.Find(CustName);

	// '������� * ����ư�Ÿ�' ���� ���� ���� ���Ը� �湮�ؾ� �ϹǷ� (�ִ� ������� - ���� �������) ���� ���ϵ��� �Ѵ�.
	// '�ִ� ������� == ���� �������'�̾ ������ ������� �� 0�� �Ǵ� ���� �����ϱ� ���� 0.1�� �����־���.
	float BistroRateAvg = (CustomerRateComponent->MaxRate + 0.1) - (*CustomerRateComponent->CustTypeRateMap.Find(CurCustType));
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

void ACustomer::SetTaste()
{
	// ���Ƿ� "���� �������" ������ �׻� 3���� �����ϵ��� ��
	for (int i = 0; i < 3; i++) {
		int FillingIndex = UKismetMathLibrary::RandomIntegerInRange(0, IngredientManagerSystem->FillingRows.Num() - 1);
		Taste.Add(FillingIndex);
	}

	int MeatIndex = UKismetMathLibrary::RandomIntegerInRange(0, IngredientManagerSystem->MeatRows.Num() - 1);
	Taste.Add(MeatIndex);

	int SauceIndex = UKismetMathLibrary::RandomIntegerInRange(0, IngredientManagerSystem->SauceRows.Num() - 1);
	Taste.Add(SauceIndex);
}

int32 ACustomer::CountNotTasteNum(ASandwich* Sandwich)
{
	// ������ �ƴ� ��� ����
	int32 NotTasteNum = 0;

	Taste.Sort([](const int32 A, const int32 B)
		{
			return A < B;
		});

	Sandwich->Ingredients.Sort([](const int32 A, const int32 B)
		{
			return A < B;
		});

	// ù ��° ��ᰡ ���� ��
	if (Sandwich->Ingredients[0] == IngredientManagerSystem->BreadIndex) {
		Sandwich->Ingredients.Remove(0);
	}
	else {
		NotTasteNum++;
	}

	const int IngrNum = Sandwich->Ingredients.Num();
	// ������ ��ᰡ ���� ��
	if (Sandwich->Ingredients[IngrNum - 1] == IngredientManagerSystem->BreadIndex) {
		Sandwich->Ingredients.Remove(IngrNum - 1);
	}
	else {
		NotTasteNum++;
	}

	for (int i = 0; i < IngrNum; i++) {

		// ������ġ ��ᰡ �մ��� ������ �ƴ϶��
		if ((Taste.Contains(Sandwich->Ingredients[i])) == false) {
			NotTasteNum++;
		}
		// ������ġ ��ᰡ �մ��� �����̶��
		else {
			// �ߺ� ������ ���� ã�� ������ ����
			int32 Ingr = Sandwich->Ingredients[i];
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

	ReviewRate += Score;
}

void ACustomer::AddDessertReview()
{
	const int32 DessertBonus = 10;
	ReviewRate += DessertBonus;
}

void ACustomer::EatSandwich()
{
	Eat(10.0f);
	
	/*�մԴ�� ��� �ʿ�*/

	PlayerBistro->UpdateCustomerReviewAvg(ReviewRate);
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
}

void ACustomer::Eat(float EatingTime)
{
	/*�ʿ�� ������ �Դ� �ִϸ��̼� �߰�*/

	IsEat = true;
	DestroyTimer = true;
	LeaveDelayTime = EatingTime;
}
