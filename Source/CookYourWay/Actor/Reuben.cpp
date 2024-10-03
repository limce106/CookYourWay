// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Reuben.h"
#include "GameFramework/CharacterMovementComponent.h"

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

void AReuben::AttachToSocket(AActor* Actor)
{
	Actor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HoldSocket"));
}

void AReuben::DetachActorFromSocket()
{
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);

	AttachedActors[0]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
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
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);

	// ���ó� ����������� ���.
	if (InteractActor->GetClass() == BP_Sandwich || InteractActor->GetClass() == BP_CookingUtensil) {
		AttachToSocket(InteractActor);
	}

	// Ÿ�� ���� ������ ����� ���.
	else if (InteractActor->GetClass() == BP_Ingredient) {
		AIngredient* Ingredient = Cast<AIngredient>(InteractActor);
		if (Ingredient->IsCooked() && !Ingredient->IsBurn) {
			AttachToSocket(Ingredient);
		}
	}

	// ���ø� �����Ͽ� �÷��̾ ��� �Ѵ�.
	else if (InteractActor->GetClass() == BP_Plates) {
		ASandwich* Sandwich = GetWorld()->SpawnActor<ASandwich>(BP_Sandwich, GetMesh()->GetSocketLocation("HoldSocket"), GetMesh()->GetSocketRotation("HoldSocket"));
		AttachToSocket(Sandwich);
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
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);

	ASandwich* HoldingSandwich = Cast<ASandwich>(AttachedActors[0]);

	// Ÿ�� ����, ������ ����� ����/������ġ ���� �ø���.
	if (InteractActor->GetClass() == BP_Ingredient) {
		AIngredient* Ingredient = Cast<AIngredient>(InteractActor);
		if (Ingredient->IsCooked() && !Ingredient->IsBurn) {
			HoldingSandwich->AddIngredient(Ingredient);
		}
	}

	// Ÿ�� ����, ������ ��ᰡ �������� ���� �ִٸ� ����/������ġ ���� �ø���.
	else if (InteractActor->GetClass() == BP_CookingUtensil) {
		ACookingUtensil* CookingUtensil = Cast<ACookingUtensil>(InteractActor);
		if (CookingUtensil->PlacedIngredient->IsCooked() && !CookingUtensil->PlacedIngredient->IsBurn) {
			HoldingSandwich->AddIngredient(CookingUtensil->PlacedIngredient);
		}
	}

	// ���̺� ���� �ƹ��͵� ���ٸ� ����/������ġ�� ���̺� ���� �ø���.
	else if (InteractActor->GetClass() == BP_Table) {
		ATable* Table = Cast<ATable>(InteractActor);
		if (!Table->IsActorOn) {
			Table->PutActorOn(AttachedActors[0]);
		}
	}
}

void AReuben::CookingUtensilOnSocketInteraction(AActor* InteractActor)
{
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);

	ACookingUtensil* HoldingCookingUtensil = Cast<ACookingUtensil>(AttachedActors[0]);

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
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);

	AIngredient* HoldingIngr = Cast<AIngredient>(AttachedActors[0]);

	// Ÿ�� ����, ������ ����� ����/������ġ ���� �ø���.
	if (InteractActor->GetClass() == BP_Sandwich) {
		ASandwich* Sandwich = Cast<ASandwich>(InteractActor);
		if (HoldingIngr->IsCooked() && !HoldingIngr->IsBurn) {
			Sandwich->AddIngredient(HoldingIngr);
		}
	}

	// �������� ���� ��ᰡ ���ٸ� ��Ḧ �������� ���� �ø���.
	else if (InteractActor->GetClass() == BP_CookingUtensil) {
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
	if (IsHold == false) {
		EmptyOnSocketInteraction(OverlappedActor);
	}
	else {
		TArray<AActor*> AttachedActors;
		GetAttachedActors(AttachedActors);

		// ��� �ִ� ���� ������.
		if (OverlappedActor->GetClass() == BP_TrashBin) {
			AttachedActors[0]->Destroy();
			return;
		}

		if (AttachedActors[0]->GetClass() == BP_Sandwich) {
			SandwichOnSocketInteraction(OverlappedActor);
		}
		else if (AttachedActors[0]->GetClass() == BP_CookingUtensil) {
			CookingUtensilOnSocketInteraction(OverlappedActor);
		}
		else if (AttachedActors[0]->GetClass() == BP_Ingredient) {
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
