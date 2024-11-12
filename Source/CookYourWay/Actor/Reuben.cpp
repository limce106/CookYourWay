// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Reuben.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Subsystems/PanelExtensionSubsystem.h>
#include <Widget/IngredientBoardWidget.h>
#include <Kismet/GameplayStatics.h>
#include "PlayerBistro.h"
#include <Components/ShapeComponent.h>

AReuben::AReuben()
{
	PrimaryActorTick.bCanEverTick = true;

	// ��Ʈ�ѷ� ȸ�� ��� �� ��
	bUseControllerRotationYaw = false;
	// �̵��� �� �̵� �������� ȸ��
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AReuben::BeginPlay()
{
	Super::BeginPlay();
	
}

void AReuben::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHeldActorLoc();
}

void AReuben::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f)) {
		// ��Ʈ�ѷ� ȸ�� �� ��������
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw - 90.0f, 0);

		// ���� ����(X��) ���
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// �̵� �Է� ����
		AddMovementInput(Direction, Value);
	}
}

void AReuben::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f)) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw - 90.0f, 0);

		// ������ ����(Y��) ���
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AReuben::HoldActor(AActor* Actor)
{
	UShapeComponent* ActorCollision = Cast<UShapeComponent>(Actor->GetComponentByClass(UShapeComponent::StaticClass()));
	ActorCollision->SetCollisionProfileName(TEXT("NoCollision"));

	HeldActor = Actor;
	IsHold = true;
}

void AReuben::PutDownActor()
{
	UPrimitiveComponent* ActorCollision = Cast<UPrimitiveComponent>(HeldActor->GetComponentByClass(UPrimitiveComponent::StaticClass()));
	ActorCollision->SetCollisionProfileName(TEXT("BlockAll"));

	HeldActor = nullptr;
	IsHold = false;
}

void AReuben::SetHeldActorLoc()
{
	if (HeldActor) {
		FVector LeftHandLoc = GetMesh()->GetSocketLocation(FName("LeftHandSocket"));
		FVector RightHandLoc = GetMesh()->GetSocketLocation(FName("RightHandSocket"));

		FVector MidHandLoc = (LeftHandLoc + RightHandLoc) / 2;
		MidHandLoc.Z += 15.0f;

		HeldActor->SetActorLocation(MidHandLoc);
	}
}

void AReuben::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AReuben::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AReuben::MoveRight);

	InputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this, &AReuben::Interaction);
	InputComponent->BindAction("Chop", EInputEvent::IE_Pressed, this, &AReuben::Chop);
}

void AReuben::EmptyOnSocketInteraction(AActor* InteractActor)
{
	// ���ó� ����������� ���.
	if (InteractActor->GetClass() == BP_Sandwich || InteractActor->GetClass()->IsChildOf(ACookingUtensil::StaticClass())) {
		HoldActor(InteractActor);
	}

	// Ÿ�� ���� ������ ����� ���.
	else if (InteractActor->GetClass() == BP_Ingredient) {
		AIngredient* Ingredient = Cast<AIngredient>(InteractActor);
		if (Ingredient->IsCooked() && !Ingredient->IsBurn) {
			HoldActor(Ingredient);
		}
	}

	// ���ø� �����Ͽ� �÷��̾ ��� �Ѵ�.
	else if (InteractActor->GetClass() == BP_Plates) {
		ASandwich* Sandwich = GetWorld()->SpawnActor<ASandwich>(BP_Sandwich, GetActorLocation(), GetActorRotation());
		HoldActor(Sandwich);
	}

	// ���̺��� ����ִٸ� ���� �д�.
	else if (InteractActor->GetClass() == BP_Table) {
		ATable* Table = Cast<ATable>(InteractActor);
		if (Table->IsActorOn) {
			Table->PickUpActor();
		}
	}
}

void AReuben::SandwichOnSocketInteraction(AActor* InteractActor)
{
	ASandwich* HoldingSandwich = Cast<ASandwich>(HeldActor);

	// Ÿ�� ����, ������ ����� ����/������ġ ���� �ø���.
	if (InteractActor->GetClass() == BP_Ingredient) {
		AIngredient* Ingredient = Cast<AIngredient>(InteractActor);
		if (Ingredient->IsCooked() && !Ingredient->IsBurn) {
			HoldingSandwich->AddIngredient(Ingredient);
		}
	}

	// Ÿ�� ����, ������ ��ᰡ �������� ���� �ִٸ� ����/������ġ ���� �ø���.
	else if (InteractActor->GetClass()->IsChildOf(ACookingUtensil::StaticClass())) {
		ACookingUtensil* CookingUtensil = Cast<ACookingUtensil>(InteractActor);
		if (CookingUtensil->PlacedIngredient->IsCooked() && !CookingUtensil->PlacedIngredient->IsBurn) {
			HoldingSandwich->AddIngredient(CookingUtensil->PlacedIngredient);
		}
	}

	// ���̺� ���� �ƹ��͵� ���ٸ� ����/������ġ�� ���̺� ���� �ø���.
	else if (InteractActor->GetClass() == BP_Table) {
		ATable* Table = Cast<ATable>(InteractActor);
		if (!Table->IsActorOn) {
			Table->PutActorOn(HeldActor);
		}
	}
}

void AReuben::CookingUtensilOnSocketInteraction(AActor* InteractActor)
{
	ACookingUtensil* HoldingCookingUtensil = Cast<ACookingUtensil>(HeldActor);

	// �������� ���� ��ᰡ ���ٸ� ��Ḧ �������� ���� �ø���.
	if (InteractActor->GetClass() == BP_Ingredient) {
		if (!HoldingCookingUtensil->IsIngredientOn) {
			AIngredient* Ingredient = Cast<AIngredient>(InteractActor);
			HoldingCookingUtensil->PutIngrOn(Ingredient);
		}
	}

	// �������� ���� ������ ��ᰡ �ְ� Ÿ�� �ʾҴٸ� ��Ḧ ����/������ġ ���� �ø���.
	else if (InteractActor->GetClass() == BP_Sandwich) {
		ASandwich* Sandwich = Cast<ASandwich>(InteractActor);
		if (HoldingCookingUtensil->IsIngredientOn && !HoldingCookingUtensil->PlacedIngredient->IsBurn) {
			Sandwich->AddIngredient(HoldingCookingUtensil->PlacedIngredient);
		}
	}

	// ���̺� ���� �ƹ��͵� ���ٸ� ���������� ���̺� ���� �ø���.
	else if (InteractActor->GetClass() == BP_Table) {
		ATable* Table = Cast<ATable>(InteractActor);
		if (!Table->IsActorOn) {
			Table->PutActorOn(HoldingCookingUtensil);
		}
	}
}

void AReuben::IngrOnSocketInteraction(AActor* InteractActor)
{
	AIngredient* HoldingIngr = Cast<AIngredient>(HeldActor);

	// Ÿ�� ����, ������ ����� ����/������ġ ���� �ø���.
	if (InteractActor->GetClass() == BP_Sandwich) {
		ASandwich* Sandwich = Cast<ASandwich>(InteractActor);
		if (HoldingIngr->IsCooked() && !HoldingIngr->IsBurn) {
			Sandwich->AddIngredient(HoldingIngr);
		}
	}

	// �������� ���� ��ᰡ ���ٸ� ��Ḧ �������� ���� �ø���.
	else if (InteractActor->GetClass()->IsChildOf(ACookingUtensil::StaticClass())) {
		ACookingUtensil* CookingUtensil = Cast<ACookingUtensil>(InteractActor);
		if (!CookingUtensil->IsIngredientOn) {
			CookingUtensil->PutIngrOn(HoldingIngr);
		}
	}

	// ���̺� ���� �ƹ��͵� ���ٸ� ��Ḧ ���̺� ���� �ø���.
	else if (InteractActor->GetClass() == BP_Table) {
		ATable* Table = Cast<ATable>(InteractActor);
		if (!Table->IsActorOn) {
			Table->PutActorOn(HoldingIngr);
		}
	}
}

void AReuben::Interaction()
{
	if (!OverlappedActor || !IsOverlappingActor(OverlappedActor)) {
		return;
	}

	// �մ�
	if (OverlappedActor->GetClass() == BP_Customer) {
		ACustomer* Customer = Cast<ACustomer>(OverlappedActor);
		TryGiveSomething(Customer);
	}
	// �����
	else if (OverlappedActor->GetClass() == BP_Fridge) {
		UIngredientBoardWidget* BP_IngredientBoard = CreateWidget<UIngredientBoardWidget>(GetWorld(), BP_IngredientBoardClass);
		if (BP_IngredientBoard) {
			BP_IngredientBoard->AddToViewport();
		}
		return;
	}

	// �տ� �ƹ��͵� �� ��� ���� ��
	if (IsHold == false) {
		EmptyOnSocketInteraction(OverlappedActor);
	}
	// �տ� ���� ��� ���� ��
	else {
		// �տ� ��� �ִ� ���� ������.
		if (OverlappedActor->GetClass() == BP_TrashBin) {
			HeldActor->Destroy();
			IsHold = false;
			return;
		}

		// �տ� ��� �ִ� ���� ������ġ
		if (HeldActor->GetClass() == BP_Sandwich) {
			SandwichOnSocketInteraction(OverlappedActor);
		}
		// �տ� ��� �ִ� ���� ��������
		else if (HeldActor->GetClass()->IsChildOf(ACookingUtensil::StaticClass())) {
			CookingUtensilOnSocketInteraction(OverlappedActor);
		}
		// �տ� ��� �ִ� ���� ���
		else if (HeldActor->GetClass() == BP_Ingredient) {
			IngrOnSocketInteraction(OverlappedActor);
		}
	}
}

void AReuben::Chop()
{
	if (OverlappedActor->GetClass() == BP_CuttingBoard) {
		ACuttingBoard* CuttingBoard = Cast<ACuttingBoard>(OverlappedActor);
		CuttingBoard->Chop();
	}
}

void AReuben::TryGiveSomething(ACustomer* Customer)
{
	// �տ� �ƹ��͵� ��� ���� ������
	if (IsHold == false) {
		return;
	}

	if (HeldActor->GetClass() == BP_Sandwich) {
		GiveSandwich(Customer);
	}
	else if (HeldActor->GetClass() == BP_Dessert) {
		GiveDessert(Customer);
	}
	else {
		return;
	}
}

void AReuben::GiveSandwich(ACustomer* Customer)
{

	ASandwich* Sandwich = Cast<ASandwich>(HeldActor);
	// ������ġ�� ���� �� ���ö�� 
	if (Sandwich->Ingredients.Num() == 0) {
		return;
	}

	Customer->AddSandwichReview(Sandwich);
	Sandwich->Destroy();
	IsHold = false;

	// �մ� ���� Ÿ�̸� ����
	Customer->StartDestroyTimer();
}

void AReuben::GiveDessert(ACustomer* Customer)
{
	// �մ��� ����Ʈ�� ���� �� �ִٸ� (���� �Ļ�ð��� �����ٸ�)
	if (Customer->CanGetDessert()) {
		HeldActor->Destroy();
		IsHold = false;
		Customer->AddDessertReview();
	}
}
