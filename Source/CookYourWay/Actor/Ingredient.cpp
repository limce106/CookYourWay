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
	// 메시의 로컬 바운딩 박스에서 최소 좌표와 최대 좌표를 가져온다.
	StaticMesh->GetLocalBounds(MinBound, MaxBound);
	// 메시의 중심 위치 계산
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

	// 조리도구에 올라가지 않은, 조리 되지 않은 재료
	if (!Reuben->IsHold) {
		if (!IsCooked()) {
			Reuben->HoldActor(this);
		}
	}
	// 조리된 재료라면 접시/샌드위치 위로 올린다.
	else if (Reuben->GetHeldActorClass()->IsChildOf(ASandwich::StaticClass())) {
		if (IsCooked()) {
			ASandwich* HoldingSandwich = Cast<ASandwich>(Reuben->HeldActor);
			HoldingSandwich->AddIngredient(this);
		}
	}
	// 조리도구 위에 재료가 없다면 재료를 조리도구 위로 올린다.
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
		OverlayMesh->DestroyComponent(); // 안전하게 제거
		OverlayMesh = nullptr;
	}

	// 새로운 StaticMeshComponent 생성
	OverlayMesh = NewObject<UStaticMeshComponent>(this);

	OverlayMesh->SetStaticMesh(StaticMeshComponent->GetStaticMesh()); // 기존 메시 복제
	OverlayMesh->SetMaterial(0, OverlayMaterial);
	OverlayMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 기존 메시보다 약간 크게 설정해서 덮어씌우기
	FVector NewScale = StaticMeshComponent->GetComponentScale() * 1.01f;
	OverlayMesh->SetWorldScale3D(NewScale);


	// 액터에 추가
	OverlayMesh->AttachToComponent(StaticMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	OverlayMesh->RegisterComponent();
}
