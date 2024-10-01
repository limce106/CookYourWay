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
	if (InteractActor->GetClass() == BP_Sandwich || InteractActor->GetClass() == BP_CookingUtensil) {
		InteractActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HoldSocket"));
	}
	// Ÿ�� ���� ������ ����� ���.
	else if (InteractActor->GetClass() == BP_Ingredient) {

	}
	else if (InteractActor->GetClass() == BP_Plates) {
		// ���ø� �����Ͽ� �÷��̾ ��� �Ѵ�.
		ASandwich* Sandwich = GetWorld()->SpawnActor<ASandwich>(BP_Sandwich, GetMesh()->GetSocketLocation("HoldSocket"), GetMesh()->GetSocketRotation("HoldSocket"));
		Sandwich->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HoldSocket"));
	}
	else if (InteractActor->GetClass() == BP_TrashBin) {
		TArray<AActor*> AttachedActors;
		GetAttachedActors(AttachedActors);
		if (AttachedActors[0]) {
			AttachedActors[0]->Destroy();
		}
	}
}

void AReuben::SandwichOnSocketInteraction(AActor* InteractActor)
{
	// Ÿ�� ����, ������ ����� ����/������ġ ���� �ø���.
	if (InteractActor->GetClass() == BP_Ingredient) {

	}
	// Ÿ�� ����, ������ ��ᰡ �������� ���� �ִٸ� ����/������ġ ���� �ø���.
	else if (InteractActor->GetClass() == BP_CookingUtensil) {

	}
	// ���̺� ���� �ƹ��͵� ���ٸ� ����/������ġ�� ���̺� ���� �ø���.
	else if (InteractActor->GetClass() == BP_Table) {

	}
}

void AReuben::CookingUtensilOnSocketInteraction(AActor* InteractActor)
{
	// �������� ���� ��ᰡ ���ٸ� ��Ḧ �������� ���� �ø���.
	if (InteractActor->GetClass() == BP_Ingredient) {

	}
	// �������� ���� ������ ��ᰡ �ְ� Ÿ�� �ʾҴٸ� ��Ḧ ����/������ġ ���� �ø���.
	else if (InteractActor->GetClass() == BP_Sandwich) {

	}
	// ���̺� ���� �ƹ��͵� ���ٸ� ���������� ���̺� ���� �ø���.
	else if (InteractActor->GetClass() == BP_Table) {

	}
}

void AReuben::IngrOnSocketInteraction(AActor* InteractActor)
{
	// Ÿ�� ����, ������ ����� ����/������ġ ���� �ø���.
	if (InteractActor->GetClass() == BP_Sandwich) {

	}
	// ��Ḧ �������� ���� �ø���.
	else if (InteractActor->GetClass() == BP_CookingUtensil) {

	}
	// ���̺� ���� �ƹ��͵� ���ٸ� ��Ḧ ���̺� ���� �ø���.
	else if (InteractActor->GetClass() == BP_Table) {
		ATable* Table = Cast<ATable>(InteractActor);
		if (!Table->IsActorOn) {
			TArray<AActor*> AttachedActors;
			GetAttachedActors(AttachedActors);
			Table->PutActorOn(AttachedActors[0]);
		}
	}
}

void AReuben::Interaction()
{
	if (IsHold == false) {
		EmptyOnSocketInteraction(OverlappedActor);
	}
	else {
		TArray<USceneComponent*> HoldScoketComponents = GetMesh()->GetAttachChildren();
		if (HoldScoketComponents[0]->GetClass() == BP_Sandwich) {
			SandwichOnSocketInteraction(OverlappedActor);
		}
		else if (HoldScoketComponents[0]->GetClass() == BP_CookingUtensil) {
			CookingUtensilOnSocketInteraction(OverlappedActor);
		}
		else if (HoldScoketComponents[0]->GetClass() == BP_Ingredient) {
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
