// Fill out your copyright notice in the Description page of Project Settings.


#include "Customer.h"
#include <Kismet/GameplayStatics.h>
#include "Competitor.h"
#include "PlayerBistro.h"
#include"Components/CapsuleComponent.h"

ACustomer::ACustomer()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACustomer::Init()
{
	CustName = "Amy"; // 임의로 테스트를 위해 설정, 
	SetSkeletalMesh();
	SelectBistroToVisit();

	AAIController* AINpcController = Cast<AAIController>(GetController());
	// AIMoveTo();
	AINpcController->MoveToLocation(VisitDest);
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

void ACustomer::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BP_Competitor, AllCompetitorActorArr);
	PlayerBistro = Cast<APlayerBistro>(UGameplayStatics::GetActorOfClass(GetWorld(), BP_PlayerBistro));
	Init();
}

void ACustomer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	FVector BistroLoc = Bistro->GetActorLocation() + FVector(200.0f, 0.0f, 0.0f);

	UCustomerRateComponent* CustomerRateComponent = Cast<UCustomerRateComponent>(Bistro->GetComponentByClass(UCustomerRateComponent::StaticClass()));
	ECustType CurCustType = *CustomerRateComponent->CustStringToTypeMap.Find(CustName);

	// '평점평균 * 맨해튼거리' 값이 가장 작은 가게를 방문해야 하므로 (최대 평점평균 - 실제 평점평균) 값을 곱하도록 한다.
	// '최대 평점평균 == 실제 평점평균'이어서 수식을 계산했을 때 0이 되는 것을 방지하기 위해 0.1을 더해주었다.
	float BistroRateAvg = (CustomerRateComponent->MaxRate + 0.1) - (*CustomerRateComponent->CustTypeRateMap.Find(CurCustType));
	float VisitRank = BistroRateAvg * (ManhattanDist(CustomerLoc, BistroLoc));

	BistroLocRankMap.Add(BistroLoc, VisitRank);

	return VisitRank;
}

void ACustomer::SelectBistroToVisit()
{
	FVector CustomerLoc = GetActorLocation();

	BistroLocRankMap.Empty();
	CalcVisitRank(PlayerBistro);

	for (auto Competitor : AllCompetitorActorArr) {
		CalcVisitRank(Competitor);
	}

	// 방문 점수들을 오름차순으로 정렬
	BistroLocRankMap.ValueSort([](float A, float B)
	{
		return A < B;
	});

	TArray<FVector> BistroLocRankMapKeys;
	BistroLocRankMap.GenerateKeyArray(BistroLocRankMapKeys);
	VisitDest = BistroLocRankMapKeys[0];	// 가장 점수가 낮은 가게를 목적지로 설정
	VisitDest.Y += 220.0f;
	VisitDest.Z = 85.0f;
}
