// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Reuben.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Subsystems/PanelExtensionSubsystem.h>
#include <Widget/IngredientBoardWidget.h>
#include <Kismet/GameplayStatics.h>
#include "PlayerBistro.h"
#include <Components/ShapeComponent.h>
#include "FryPan.h"

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
	
	PlayerBistro = Cast<APlayerBistro>(UGameplayStatics::GetActorOfClass(GetWorld(), BP_PlayerBistro));
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
	UShapeComponent* ActorCollision = Cast<UShapeComponent>(Actor->FindComponentByClass(UShapeComponent::StaticClass()));
	if (ActorCollision) {
		ActorCollision->SetCollisionProfileName(TEXT("NoCollision"));
	}

	HeldActor = Actor;
	IsHold = true;
}

void AReuben::PutDownActor()
{
	UPrimitiveComponent* ActorCollision = Cast<UPrimitiveComponent>(HeldActor->FindComponentByClass(UPrimitiveComponent::StaticClass()));
	if (ActorCollision) {
		if ((HeldActor->GetClass() == BP_Ingredient) || (HeldActor->GetClass() == BP_Sandwich)) {
			ActorCollision->SetCollisionProfileName(TEXT("OnSomething"));
		}
		else {
			ActorCollision->SetCollisionProfileName(TEXT("BlockAll"));
		}
	}

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
	ACuttingBoard* CuttingBoard = Cast<ACuttingBoard>(UGameplayStatics::GetActorOfClass(GetWorld(), BP_CuttingBoard));
	AFryPan* FryPan = Cast<AFryPan>(UGameplayStatics::GetActorOfClass(GetWorld(), BP_FryPan));

	// ������ġ
	if (InteractActor->GetClass() == BP_Sandwich) {
		HoldActor(InteractActor);
	}

	// ��������
	else if (InteractActor->GetClass()->IsChildOf(ACookingUtensil::StaticClass())) {
		ACookingUtensil* CookingUtensil = Cast<ACookingUtensil>(InteractActor);
		if (CookingUtensil->IsIngredientOn) {
			CookingUtensil->PickUpIngr();
		}
		else {
			HoldActor(InteractActor);
		}
	}

	// ���������� �ö��� ����, ���� ���� ���� ���
	else if (InteractActor->GetClass() == BP_Ingredient) {
		AIngredient* Ingredient = Cast<AIngredient>(InteractActor);
		if (!Ingredient->IsCooked()) {
			HoldActor(Ingredient);
		}
	}

	// ���� ����
	else if (InteractActor->GetClass() == BP_Plates) {
		ASandwich* Sandwich = GetWorld()->SpawnActor<ASandwich>(BP_Sandwich, GetActorLocation(), GetActorRotation());
		HoldActor(Sandwich);
	}

	// ���̺�
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

	// ������ ����� ����/������ġ ���� �ø���.
	if (InteractActor->GetClass() == BP_Ingredient) {
		AIngredient* Ingredient = Cast<AIngredient>(InteractActor);
		if (Ingredient->IsCooked()) {
			HoldingSandwich->AddIngredient(Ingredient);
		}
	}

	// ���� �Ϸ�� ��ᰡ �������� ���� �ִٸ� ����/������ġ ���� �ø���.
	else if (InteractActor->GetClass()->IsChildOf(ACookingUtensil::StaticClass())) {
		ACookingUtensil* CookingUtensil = Cast<ACookingUtensil>(InteractActor);
		if (CookingUtensil->PlacedIngredient->IsCooked()) {
			CookingUtensil->PickUpIngr();
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

	// �������� ���� ���� �Ϸ�� ��ᰡ �ִٸ� ��Ḧ ����/������ġ ���� �ø���.
	else if (InteractActor->GetClass() == BP_Sandwich) {
		ASandwich* Sandwich = Cast<ASandwich>(InteractActor);
		if (HoldingCookingUtensil->IsIngredientOn && HoldingCookingUtensil->PlacedIngredient->IsCooked()) {
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

	if (InteractActor->GetClass() == BP_Plates) {
		ASandwich* Sandwich = GetWorld()->SpawnActor<ASandwich>(BP_Sandwich, GetActorLocation(), GetActorRotation());
		HoldActor(Sandwich);
		if (HoldingIngr->IsCooked()) {
			Sandwich->AddIngredient(HoldingIngr);
		}
	}

	// ������ ����� ����/������ġ ���� �ø���.
	else if (InteractActor->GetClass() == BP_Sandwich) {
		ASandwich* Sandwich = Cast<ASandwich>(InteractActor);
		if (HoldingIngr->IsCooked()) {
			Sandwich->AddIngredient(HoldingIngr);
		}
	}

	// �������� ���� ��ᰡ ���ٸ� ��Ḧ �������� ���� �ø���.
	else if (InteractActor->GetClass()->IsChildOf(ACookingUtensil::StaticClass())) {
		// ����
		if ((InteractActor->GetClass() == BP_CuttingBoard) 
			&& (HoldingIngr->CurIngrData->IngrType == "Filling")) {
			ACuttingBoard* CuttingBoard = Cast<ACuttingBoard>(InteractActor);

			if (!CuttingBoard->IsIngredientOn) {
				CuttingBoard->PutIngrOn(HoldingIngr);
			}
		}
		// �Ķ�����
		else if ((InteractActor->GetClass() == BP_FryPan) && (HoldingIngr->CurIngrData->IngrType == "Meat")) {
			AFryPan* FryPan = Cast<AFryPan>(InteractActor);

			if (!FryPan->IsIngredientOn) {
				FryPan->PutIngrOn(HoldingIngr);
			}
		}
	}

	// ���̺� ���� �ƹ��͵� ���ٸ� ��Ḧ ���̺� ���� �ø���.
	else if (InteractActor->GetClass() == BP_Table) {
		ATable* Table = Cast<ATable>(InteractActor);
		if (!Table->IsActorOn) {
			Table->PutActorOn(HoldingIngr);
		}
		else {
			if (Table->PlacedActor->GetClass() == BP_Sandwich) {
				ASandwich* PlacedSandwich = Cast<ASandwich>(Table->PlacedActor);
				PlacedSandwich->AddIngredient(HoldingIngr);
			}
		}
	}
}

void AReuben::Interaction()
{
	if (!OverlappedActor || !IsOverlappingActor(OverlappedActor)) {
		return;
	}

	// �մ�
	if (OverlappedActor->GetClass() == BP_DiningTable) {
		ADiningTable* DiningTable = Cast<ADiningTable>(OverlappedActor);
		TryGiveSomething(DiningTable->SeatedCustomer);
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
			if (HeldActor->GetClass() == BP_Sandwich) {
				ASandwich* HeldSandwich = Cast<ASandwich>(HeldActor);
				HeldSandwich->DestroySandwich();
			}
			else {
				HeldActor->Destroy();
			}
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
	ACuttingBoard* CuttingBoard = Cast<ACuttingBoard>(UGameplayStatics::GetActorOfClass(GetWorld(), BP_CuttingBoard));
	if (IsOverlappingActor(CuttingBoard)) {
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
	Sandwich->DestroySandwich();
	IsHold = false;

	Customer->EatSandwich();

	GetWorld()->GetTimerManager().SetTimer(CustSandwichTimerHandler, FTimerDelegate::CreateLambda([&]()
		{
			PlayerBistro->LeaveAndSitNextCust(Customer);
		}), Customer->LeaveDelayTime, false);
}

void AReuben::GiveDessert(ACustomer* Customer)
{
	if (Customer->CanGetDessert()) {
		GetWorld()->GetTimerManager().ClearTimer(CustSandwichTimerHandler);
		HeldActor->Destroy();
		IsHold = false;

		Customer->EatDessert();
		Customer->AddDessertReview();

		FTimerHandle CustDessertTimerHandler;
		GetWorld()->GetTimerManager().SetTimer(CustDessertTimerHandler, FTimerDelegate::CreateLambda([&]()
			{
				PlayerBistro->LeaveAndSitNextCust(Customer);
			}), Customer->LeaveDelayTime, false);
	}
}
