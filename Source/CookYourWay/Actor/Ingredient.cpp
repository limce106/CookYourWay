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
}

void AIngredient::Init(FString IngrName)
{
	for (int i = 0; i < IngredientManagerSystem->IngredientTableRowName.Num(); i++) {
		FString IngredientTableRowName = IngredientManagerSystem->IngredientTableRowName[i].ToString();
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

void AIngredient::SetStaticMesh(FString IngredientName)
{
	FString StaticMeshPath = FString("/Game/Assets/Art_3D/Modelling/Sandwich/").Append(IngredientName).Append("_Slice").Append(".").Append(IngredientName).Append("_Slice");
	UStaticMesh* IngredientMesh = LoadObject<UStaticMesh>(NULL, *StaticMeshPath, NULL, LOAD_None, NULL);
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
	if (CurIngrData->IngrType == "Filling") {
		if (IsChopped)
			return true;
		else
			return false;
	}
	else if (CurIngrData->IngrType == "Meat") {
		if (IsRoasted)
			return true;
		else
			return false;
	}
	else {
		return true;
	}
}
