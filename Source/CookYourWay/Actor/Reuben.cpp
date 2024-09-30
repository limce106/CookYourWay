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
}

void AReuben::EmptyOnSocketInteraction(AActor* InteractActor)
{
	// ���ó� ����������� ���.
	if (InteractActor->GetClass() == BP_Plate || InteractActor->GetClass() == BP_CookingUtensil) {

		InteractActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HoldSocket"));
	}
	// ������ ����� ���.
	else if (InteractActor->GetClass() == BP_Ingredient) {

	}
}

void AReuben::PlateOnSocketInteraction(AActor* InteractActor)
{
	// Ÿ�� ����, ������ ����� ���� ���� �ø���.
	if (InteractActor->GetClass() == BP_Ingredient) {

	}
	// Ÿ�� ����, ������ ��ᰡ �������� ���� �ִٸ� ���� ���� �ø���.
	else if (InteractActor->GetClass() == BP_CookingUtensil) {

	}
	// ���̺� ���� �ƹ��͵� ���ٸ� ���ø� ���̺� ���� �ø���.
	else if (InteractActor->GetClass() == BP_Table) {

	}
}

void AReuben::CookingUtensilOnSocketInteraction(AActor* InteractActor)
{
	// �������� ���� ��ᰡ ���ٸ� ��Ḧ �������� ���� �ø���.
	if (InteractActor->GetClass() == BP_Ingredient) {

	}
	// �������� ���� ������ ��ᰡ �ְ� Ÿ�� �ʾҴٸ� ��Ḧ ���� ���� �ø���.
	else if (InteractActor->GetClass() == BP_Plate) {

	}
	// ���̺� ���� �ƹ��͵� ���ٸ� ���������� ���̺� ���� �ø���.
	else if (InteractActor->GetClass() == BP_Table) {

	}
}

void AReuben::IngrOnSocketInteraction(AActor* InteractActor)
{
	// Ÿ�� ����, ������ ����� ���� ���� �ø���.
	if (InteractActor->GetClass() == BP_Plate) {

	}
	// ��Ḧ �������� ���� �ø���.
	else if (InteractActor->GetClass() == BP_CookingUtensil) {

	}
	// ���̺� ���� �ƹ��͵� ���ٸ� ��Ḧ ���̺� ���� �ø���.
	else if (InteractActor->GetClass() == BP_Table) {

	}
}

void AReuben::Chop()
{
	if (OverlappedActor->GetClass() == BP_CuttingBoard) {
		ACuttingBoard* CuttingBoard = Cast<ACuttingBoard>(OverlappedActor);
		CuttingBoard->Chop();
	}
}
