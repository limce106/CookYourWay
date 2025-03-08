// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FryPan.h"
#include "Ingredient.h"
#include <Kismet/GameplayStatics.h>
#include "GameInstance/VillageManagerSystem.h"
#include "Reuben.h"
#include "Particles/ParticleSystemComponent.h"

void AFryPan::BeginPlay()
{
	Super::BeginPlay();

	ParticleSystemComponent = Cast<UParticleSystemComponent>(FindComponentByClass(UParticleSystemComponent::StaticClass()));
	ParticleSystemComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	ParticleSystemComponent->SetWorldScale3D(FVector(0.6f, 0.6f, 0.6f));
	ParticleSystemComponent->bAutoActivate = false;
}

float AFryPan::GetOneCookIncreasement()
{
	return (1.0f / 10.0f);
	// �׽�Ʈ
	// return (1.0f / 3.0f);
	//
}

void AFryPan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsIngredientOn && IsFrying && DelayWithDeltaTime(2.0f, DeltaTime)) {
		Fry();
	}
}

bool AFryPan::DelayWithDeltaTime(float DelayTime, float DeltaSeconds)
{
	if (TempDelayTime > DelayTime) {
		TempDelayTime = 0;
		return true;
	}
	else {
		TempDelayTime += DeltaSeconds;
		return false;
	}
}

void AFryPan::PutIngrOn(AIngredient* Ingr)
{
	Super::PutIngrOn(Ingr);

	FVector IngrLocation = GetActorLocation();
	IngrLocation.Z += 140.0f;
	Ingr->SetActorLocation(IngrLocation);

	IsFrying = true;

	UParticleSystem* SteamParticle = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/Effect/P_Steam_Lit.P_Steam_Lit"));
	ParticleSystemComponent->SetTemplate(SteamParticle);
	ParticleSystemComponent->ActivateSystem();
}

void AFryPan::Fry()
{
	if (PlacedIngredient->IsBurn) {
		return;
	}

	PlacedIngredient->CurCookRate += GetOneCookIncreasement();
	BP_CookRateWidget->CookRate += GetOneCookIncreasement();

	if (PlacedIngredient->CurCookRate == PlacedIngredient->MaxCookRate) {
		AddCookedMaterialOverlay(false);
	}
	else if (PlacedIngredient->CurCookRate > PlacedIngredient->MaxCookRate) {
		OverCookedTime--;
		// �ִ� ���� �������� 3�� �� �������� �¿� ó��
		if (OverCookedTime == 0) {
			IsFrying = false;
			PlacedIngredientBurnt();
		}
	}
}

void AFryPan::FryPanInteraction()
{
	bool InteractionSuccess = CommonCookingUtensilInteraction();
	if (InteractionSuccess) {
		ParticleSystemComponent->SetTemplate(nullptr);
		return;
	}

	if (Reuben->IsHold && Reuben->GetHeldActorClass()->IsChildOf(AIngredient::StaticClass())) {
		AIngredient* HoldingIngr = Cast<AIngredient>(Reuben->HeldActor);
		if (HoldingIngr->CurIngrData.IngrType == "Meat" && !this->IsIngredientOn) {
			PutIngrOn(HoldingIngr);
		}
	}
}

void AFryPan::PlacedIngredientBurnt()
{
	PlacedIngredient->IsBurn = true;
	AddCookedMaterialOverlay(true);

	UParticleSystem* FireParticle = LoadObject<UParticleSystem>(nullptr, TEXT("/Game/Effect/P_Fire.P_Fire"));
	ParticleSystemComponent->SetTemplate(FireParticle);
}

void AFryPan::AddCookedMaterialOverlay(bool IsBurn)
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

	UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(PlacedIngredient->FindComponentByClass(UStaticMeshComponent::StaticClass()));

	// ���ο� StaticMeshComponent ����
	if (OverlayMesh == nullptr) {
		OverlayMesh = NewObject<UStaticMeshComponent>(this);
	}

	OverlayMesh->SetStaticMesh(StaticMeshComponent->GetStaticMesh()); // ���� �޽� ����
	OverlayMesh->SetMaterial(0, OverlayMaterial);
	OverlayMesh->SetWorldTransform(StaticMeshComponent->GetComponentTransform()); // ���� �޽ÿ� ������ ��ġ

	// ���� �޽ú��� �ణ ũ�� �����ؼ� ������
	FVector NewScale = StaticMeshComponent->GetComponentScale() * 1.01f;
	OverlayMesh->SetWorldScale3D(NewScale);

	OverlayMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ���Ϳ� �߰�
	OverlayMesh->RegisterComponent();
	OverlayMesh->AttachToComponent(StaticMeshComponent, FAttachmentTransformRules::KeepWorldTransform);
}
