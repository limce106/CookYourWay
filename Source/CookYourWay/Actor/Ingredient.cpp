// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Ingredient.h"
#include <Kismet/GameplayStatics.h>
#include "Reuben.h"

AIngredient::AIngredient()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshPivot = CreateDefaultSubobject<USceneComponent>(TEXT("MeshPivot"));
	SetRootComponent(MeshPivot);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(MeshPivot);

	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMesh->SetWorldScale3D(FVector(1.0, 1.0, 1.5));

	FString BurntMeatMaterialPath = TEXT("/Game/Material/M_BurntMeat.M_BurntMeat");
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(*BurntMeatMaterialPath);
	BurntMeatMaterial = MaterialFinder.Object;
}

void AIngredient::Init(FString IngrName, bool IsSliced)
{
	for (int i = 0; i < IngredientManagerSystem->IngredientTableRowNames.Num(); i++) {
		FString IngredientTableRowName = IngredientManagerSystem->IngredientTableRowNames[i].ToString();
		if (IngrName == IngredientTableRowName) {
			CurIngrData = *IngredientManagerSystem->IngredientTableRows[i];
			break;
		}
	}

	SetStaticMeshAndPivot(IngrName, IsSliced);
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
	if (CurCookRate >= MaxCookRate)
		return true;
	else
		return false;
}

void AIngredient::IngredientInteraction()
{
	AReuben* Reuben = Cast<AReuben>(UGameplayStatics::GetPlayerPawn(this, 0));

	// ���������� �ö��� ����, ���� ���� ���� ���
	if (!Reuben->IsHold) {
		if (!IsCooked()) {
			Reuben->HoldActor(this);
		}
	}
	// ������ ����� ����/������ġ ���� �ø���.
	else if (Reuben->GetHeldActorClass()->IsChildOf(ASandwich::StaticClass())) {
		if (IsCooked()) {
			ASandwich* HoldingSandwich = Cast<ASandwich>(Reuben->HeldActor);
			HoldingSandwich->AddIngredient(this);
		}
	}
	// �������� ���� ��ᰡ ���ٸ� ��Ḧ �������� ���� �ø���.
	else if (Reuben->GetHeldActorClass()->IsChildOf(ACookingUtensil::StaticClass())) {
		ACookingUtensil* HoldingCookingUtensil = Cast<ACookingUtensil>(Reuben->HeldActor);
		if (!HoldingCookingUtensil->IsIngredientOn) {
			HoldingCookingUtensil->PutIngrOn(this);
		}
	}
}

void AIngredient::ReplaceBurntMeatMaterial()
{
	int32 MaterialNum = StaticMesh->GetNumMaterials();
	for (int i = 0; i < MaterialNum; i++) {
		StaticMesh->SetMaterial(i, BurntMeatMaterial);
	}
}
