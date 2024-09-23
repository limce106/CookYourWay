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

void AReuben::AttachToCarrySocket()
{
	//if (PlateMesh)  // PlateMesh�� ������ Static Mesh
	//{
	//	// PlateSocket�̶�� �̸��� ���Ͽ� ���� ����
	//	PlateMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("PlateSocket"));

	//	// ���Ͽ��� ������� ��ġ�� ȸ���� �����Ͽ� ����
	//	PlateMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));  // �ʿ信 ���� ��ġ ����
	//	PlateMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));  // �ʿ信 ���� ȸ�� ����
	//}
}