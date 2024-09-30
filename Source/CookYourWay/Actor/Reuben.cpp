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

void AReuben::EmptyOnSocketInteraction(AActor* InteractActor)
{
	// 접시나 조리도구라면 든다.
	if (InteractActor->GetClass() == BP_Plate || InteractActor->GetClass() == BP_CookingUtensil) {

		InteractActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HoldSocket"));
	}
	// 조리된 재료라면 든다.
	else if (InteractActor->GetClass() == BP_Ingredient) {

	}
}

void AReuben::PlateOnSocketInteraction(AActor* InteractActor)
{
	// 타지 않은, 조리된 재료라면 접시 위로 올린다.
	if (InteractActor->GetClass() == BP_Ingredient) {

	}
	// 타지 않은, 조리된 재료가 조리도구 위에 있다면 접시 위로 올린다.
	else if (InteractActor->GetClass() == BP_CookingUtensil) {

	}
	// 테이블 위에 아무것도 없다면 접시를 테이블 위로 올린다.
	else if (InteractActor->GetClass() == BP_Table) {

	}
}

void AReuben::CookingUtensilOnSocketInteraction(AActor* InteractActor)
{
	// 조리도구 위에 재료가 없다면 재료를 조리도구 위로 올린다.
	if (InteractActor->GetClass() == BP_Ingredient) {

	}
	// 조리도구 위에 조리된 재료가 있고 타지 않았다면 재료를 접시 위로 올린다.
	else if (InteractActor->GetClass() == BP_Plate) {

	}
	// 테이블 위에 아무것도 없다면 조리도구를 테이블 위로 올린다.
	else if (InteractActor->GetClass() == BP_Table) {

	}
}

void AReuben::IngrOnSocketInteraction(AActor* InteractActor)
{
	// 타지 않은, 조리된 재료라면 접시 위로 올린다.
	if (InteractActor->GetClass() == BP_Plate) {

	}
	// 재료를 조리도구 위로 올린다.
	else if (InteractActor->GetClass() == BP_CookingUtensil) {

	}
	// 테이블 위에 아무것도 없다면 재료를 테이블 위로 올린다.
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
