// Fill out your copyright notice in the Description page of Project Settings.


#include "Customer.h"
#include <Kismet/GameplayStatics.h>
#include "Competitor.h"

float ACustomer::ManhattanDist(FVector Loc1, FVector Loc2)
{
	float absX = abs(Loc1.X - Loc2.X);
	float absY = abs(Loc1.Y - Loc2.Y);
	return (absX + absY) / 100;
}

float ACustomer::CalcVisitRank(AActor* Bistro)
{
	FVector CustomerLoc = GetActorLocation();
	FVector BistroLoc = Bistro->GetActorLocation() + FVector(200, 0, 0);

	UCustomerRateComponent* CustomerRateComponent = Cast<UCustomerRateComponent>(Bistro->GetComponentByClass(UCustomerRateComponent::StaticClass()));
	ECustType CurCustType = *CustomerRateComponent->CustStringToTypeMap.Find(CustName);

	// '평점평균 * 맨해튼거리' 값이 가장 작은 가게를 방문해야 하므로 (최대 평점평균 - 실제 평점평균) 값을 곱하도록 한다.
	// '최대 평점평균 == 실제 평점평균'이어서 수식을 계산했을 때 0이 되는 것을 방지하기 위해 0.1을 더해주었다.
	float BistroRateAvg = (CustomerRateComponent->MaxRate + 0.1) - (*CustomerRateComponent->CustTypeRateMap.Find(CurCustType));

	return BistroRateAvg * (ManhattanDist(CustomerLoc, BistroLoc));	// 평점평균 * 가게까지의 맨해튼거리
}

void ACustomer::SelectBistroToVisit()
{
	FVector CustomerLoc = GetActorLocation();

	TSet<float> BistroVisitRank;	// 가게 방문 순위
	BistroVisitRank.Add(CalcVisitRank(PlayerBistro));

	for (auto Competitor : AllCompetitorActorArr) {
		BistroVisitRank.Add(CalcVisitRank(Competitor));
	}
}

ACustomer::ACustomer()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACustomer::Init()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BP_Competitor, AllCompetitorActorArr);
	PlayerBistro = Cast<APlayerBistro>(UGameplayStatics::GetActorOfClass(GetWorld(), BP_PlayerBistro));

	SelectBistroToVisit();
}

void ACustomer::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACustomer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACustomer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

