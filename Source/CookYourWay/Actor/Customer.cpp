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
	// 스켈레탈 메시 적용
	FString SkeletalMeshPath = FString("/Game/Assets/Art_3D/Modelling/Npc/").Append(CustName).Append("/").Append(CustName).Append(".").Append(CustName);
	USkeletalMesh* CustSkeletalMesh = LoadObject<USkeletalMesh>(NULL, *SkeletalMeshPath, NULL, LOAD_None, NULL);
	GetMesh()->SetSkeletalMesh(CustSkeletalMesh);

	// 애니메이션 블루프린트 클래스 적용
	// 에디터에서만 적용되고 빌드 시 안 될 수 있으니 꼭 확인!!
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


	if (IsWaiting) {
		// 40초 후 인내심은 0이 된다.
		Patience -= 2.5;

		if (Patience <= 0) {
			PlayerBistro->LeaveWaitingCust(this);
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

	// '평점평균 * 맨해튼거리' 값이 가장 작은 가게를 방문해야 하므로 (최대 평점평균 - 실제 평점평균) 값을 곱하도록 한다.
	// '최대 평점평균 == 실제 평점평균'일 때 BistroRateAvg 값이 0이 되어 ManhattanDist 함수 값에 상관없이 VisitRank이 0이 되는 것을 방지하기 위해 0.1을 더해주었다.
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

	// 방문 점수들을 오름차순으로 정렬
	BistroLocRankMap.ValueSort([](const float A, const float B)
	{
		return A < B;
	});

	TArray<FVector> BistroLocRankMapKeys;
	BistroLocRankMap.GenerateKeyArray(BistroLocRankMapKeys);
	VisitDest = BistroLocRankMapKeys[0];	// 가장 점수가 낮은 가게를 목적지로 설정
	VisitDest.Y += 100.0f;
	VisitDest.Z = 95.0f;
}

int32 ACustomer::CountNotTasteNum(ASandwich* Sandwich)
{
	TArray<int32> Taste = VillageManager->GetCustTaste(CustName);
	// 취향이 아닌 재료 개수
	int32 NotTasteNum = 0;

	Taste.Sort([](const int32 A, const int32 B)
		{
			return A < B;
		});

	Sandwich->Ingredients.Sort([](const int32 A, const int32 B)
		{
			return A < B;
		});

	// 첫 번째 재료가 빵일 때
	if (Sandwich->Ingredients[0] == IngredientManagerSystem->BreadIndex) {
		Sandwich->Ingredients.Remove(0);
	}
	else {
		NotTasteNum++;
	}

	const int IngrNum = Sandwich->Ingredients.Num();
	// 마지막 재료가 빵일 때
	if (Sandwich->Ingredients[IngrNum - 1] == IngredientManagerSystem->BreadIndex) {
		Sandwich->Ingredients.Remove(IngrNum - 1);
	}
	else {
		NotTasteNum++;
	}

	for (int i = 0; i < IngrNum; i++) {

		// 샌드위치 재료가 손님의 취향이 아니라면
		if ((Taste.Contains(Sandwich->Ingredients[i])) == false) {
			NotTasteNum++;
		}
		// 샌드위치 재료가 손님의 취향이라면
		else {
			// 중복 방지를 위해 찾은 취향은 제거
			int32 Ingr = Sandwich->Ingredients[i];
			// 손님이 한 종류의 재료를 두 개 이상 선택했을 가능성을 고려하여, 같은 재료가 여러 개 있어도 하나만 삭제
			Taste.RemoveSingle(Ingr);
		}
	}

	// 포함되지 않은 취향 개수만큼 더한다.
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
	
	/*손님대사 출력 필요*/

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
	/*필요시 음식을 먹는 애니메이션 추가*/

	IsEat = true;
	DestroyTimer = true;
	LeaveDelayTime = EatingTime;
}
