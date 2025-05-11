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

	SetMesh(IngrName, IsSliced);
}

void AIngredient::SetScale()
{
	if (CurIngrData.IngrType == "Filling") {
		StaticMesh->SetWorldScale3D(FVector(3.0f, 3.0f, 3.0f));
	}
	else {
		StaticMesh->SetWorldScale3D(FVector(1.5f, 1.5f, 1.5f));
	}
}

void AIngredient::SetPivotCenter()
{
	FVector MinBound, MaxBound;
	// �޽��� ���� �ٿ�� �ڽ����� �ּ� ��ǥ�� �ִ� ��ǥ�� �����´�.
	StaticMesh->GetLocalBounds(MinBound, MaxBound);
	// �޽��� �߽� ��ġ ���
	FVector MeshCenter = ((MinBound + MaxBound) / 2.0f) * StaticMesh->GetComponentScale();
	StaticMesh->SetRelativeLocation(-MeshCenter);
}

void AIngredient::SetMesh(FString IngrName, bool IsSliced)
{
	UStaticMesh* IngredientMesh = IngredientManagerSystem->GetIngrModel(IngrName, IsSliced);
	StaticMesh->SetStaticMesh(IngredientMesh);

	SetScale();
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

void AIngredient::AddBurntMaterialOverlay()
{
	UMaterialInterface* OverlayMaterial;
	if (IsBurn) {
		OverlayMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Material/M_BurntMeat.M_BurntMeat"));
	}
	else {
		OverlayMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Material/M_CookedMeat.M_CookedMeat"));
	}

	if (!OverlayMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("Brown overlay material not found!"));
		return;
	}

	UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(FindComponentByClass(UStaticMeshComponent::StaticClass()));
	if (!StaticMeshComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Static mesh component not found on ingredient"));
		return;
	}

	if (OverlayMesh)
	{
		OverlayMesh->DestroyComponent(); // �����ϰ� ����
		OverlayMesh = nullptr;
	}

	// ���ο� StaticMeshComponent ����
	OverlayMesh = NewObject<UStaticMeshComponent>(this);

	OverlayMesh->SetStaticMesh(StaticMeshComponent->GetStaticMesh()); // ���� �޽� ����
	OverlayMesh->SetMaterial(0, OverlayMaterial);
	OverlayMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ���� �޽ú��� �ణ ũ�� �����ؼ� ������
	FVector NewScale = StaticMeshComponent->GetComponentScale() * 1.01f;
	OverlayMesh->SetWorldScale3D(NewScale);


	// ���Ϳ� �߰�
	OverlayMesh->AttachToComponent(StaticMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	OverlayMesh->RegisterComponent();
}
