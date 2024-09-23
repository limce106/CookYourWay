// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Reuben.h"
#include "GameFramework/CharacterMovementComponent.h"

AReuben::AReuben()
{
	PrimaryActorTick.bCanEverTick = true;

	// 컨트롤러 회전 사용 안 함
	bUseControllerRotationYaw = false;
	// 이동할 때 이동 방향으로 회전
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
		// 컨트롤러 회전 값 가져오기
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw - 90.0f, 0);

		// 전진 방향(X축) 계산
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// 이동 입력 적용
		AddMovementInput(Direction, Value);
	}
}

void AReuben::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f)) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw - 90.0f, 0);

		// 오른쪽 방향(Y축) 계산
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