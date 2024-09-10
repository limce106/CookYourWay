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

	// '������� * ����ư�Ÿ�' ���� ���� ���� ���Ը� �湮�ؾ� �ϹǷ� (�ִ� ������� - ���� �������) ���� ���ϵ��� �Ѵ�.
	// '�ִ� ������� == ���� �������'�̾ ������ ������� �� 0�� �Ǵ� ���� �����ϱ� ���� 0.1�� �����־���.
	float BistroRateAvg = (CustomerRateComponent->MaxRate + 0.1) - (*CustomerRateComponent->CustTypeRateMap.Find(CurCustType));

	return BistroRateAvg * (ManhattanDist(CustomerLoc, BistroLoc));	// ������� * ���Ա����� ����ư�Ÿ�
}

void ACustomer::SelectBistroToVisit()
{
	FVector CustomerLoc = GetActorLocation();

	TSet<float> BistroVisitRank;	// ���� �湮 ����
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

