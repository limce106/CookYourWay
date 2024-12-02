// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Ingredient.h"
#include <Kismet/GameplayStatics.h>

AIngredient::AIngredient()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshPivot = CreateDefaultSubobject<USceneComponent>(TEXT("MeshPivot"));
	MeshPivot->SetupAttachment(RootComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(MeshPivot);

	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
}

void AIngredient::Init(FString IngrName)
{
	for (int i = 0; i < IngredientManagerSystem->IngredientTableRowNames.Num(); i++) {
		FString IngredientTableRowName = IngredientManagerSystem->IngredientTableRowNames[i].ToString();
		if (IngrName == IngredientTableRowName) {
			CurIngrData = IngredientManagerSystem->IngredientTableRows[i];
			break;
		}
	}
}

void AIngredient::SetPivotCenter()
{
	FVector MinBound, MaxBound;
	// �޽��� ���� �ٿ�� �ڽ����� �ּ� ��ǥ�� �ִ� ��ǥ�� �����´�.
	StaticMesh->GetLocalBounds(MinBound, MaxBound);
	// �޽��� �߽� ��ġ ���
	FVector MeshCenter = (MinBound + MaxBound) / 2.0f;
	StaticMesh->SetRelativeLocation(-MeshCenter);
}

void AIngredient::SetStaticMeshAndPivot(FString IngrName, bool IsSliced)
{
	UStaticMesh* IngredientMesh = IngredientManagerSystem->GetIngrModel(IngrName, IsSliced);
	StaticMesh->SetStaticMesh(IngredientMesh);

	SetPivotCenter();
}

void AIngredient::BeginPlay()
{
	Super::BeginPlay();
	
	IngredientManagerSystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UIngredientManagerSystem>();
}

void AIngredient::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AIngredient::IsCooked()
{
	if (CurCookRate >= MaxCookRate && !IsBurn)
		return true;
	else
		return false;
}
