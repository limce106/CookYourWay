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

	// 접시나 조리도구라면 든다.
	if (InteractActor->GetClass() == BP_Sandwich || InteractActor->GetClass() == BP_CookingUtensil) {
		AttachToSocket(InteractActor);
	}

	// 타지 않은 조리된 재료라면 든다.
	else if (InteractActor->GetClass() == BP_Ingredient) {
		AIngredient* Ingredient = Cast<AIngredient>(InteractActor);
		if (Ingredient->IsCooked() && !Ingredient->IsBurn) {
			AttachToSocket(Ingredient);
		}
	}

	// 접시를 스폰하여 플레이어가 들게 한다.
	else if (InteractActor->GetClass() == BP_Plates) {
		ASandwich* Sandwich = GetWorld()->SpawnActor<ASandwich>(BP_Sandwich, GetMesh()->GetSocketLocation("HoldSocket"), GetMesh()->GetSocketRotation("HoldSocket"));
		AttachToSocket(Sandwich);
	}

	// 테이블이 비어있다면 위에 둔다.
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

	// 타지 않은, 조리된 재료라면 접시/샌드위치 위로 올린다.
	if (InteractActor->GetClass() == BP_Ingredient) {
		AIngredient* Ingredient = Cast<AIngredient>(InteractActor);
		if (Ingredient->IsCooked() && !Ingredient->IsBurn) {
			HoldingSandwich->AddIngredient(Ingredient);
		}
	}

	// 타지 않은, 조리된 재료가 조리도구 위에 있다면 접시/샌드위치 위로 올린다.
	else if (InteractActor->GetClass() == BP_CookingUtensil) {
		ACookingUtensil* CookingUtensil = Cast<ACookingUtensil>(InteractActor);
		if (CookingUtensil->PlacedIngredient->IsCooked() && !CookingUtensil->PlacedIngredient->IsBurn) {
			HoldingSandwich->AddIngredient(CookingUtensil->PlacedIngredient);
		}
	}

	// 테이블 위에 아무것도 없다면 접시/샌드위치를 테이블 위로 올린다.
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

	// 조리도구 위에 재료가 없다면 재료를 조리도구 위로 올린다.
	if (InteractActor->GetClass() == BP_Ingredient) {
		if (!HoldingCookingUtensil->IsIngredientOn) {
			AIngredient* Ingredient = Cast<AIngredient>(InteractActor);
			HoldingCookingUtensil->PutIngrOn(Ingredient);
		}
	}

	// 조리도구 위에 조리된 재료가 있고 타지 않았다면 재료를 접시/샌드위치 위로 올린다.
	else if (InteractActor->GetClass() == BP_Sandwich) {
		ASandwich* Sandwich = Cast<ASandwich>(InteractActor);
		if (HoldingCookingUtensil->IsIngredientOn && !HoldingCookingUtensil->PlacedIngredient->IsBurn) {
			Sandwich->AddIngredient(HoldingCookingUtensil->PlacedIngredient);
		}
	}

	// 테이블 위에 아무것도 없다면 조리도구를 테이블 위로 올린다.
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

	// 타지 않은, 조리된 재료라면 접시/샌드위치 위로 올린다.
	if (InteractActor->GetClass() == BP_Sandwich) {
		ASandwich* Sandwich = Cast<ASandwich>(InteractActor);
		if (HoldingIngr->IsCooked() && !HoldingIngr->IsBurn) {
			Sandwich->AddIngredient(HoldingIngr);
		}
	}

	// 조리도구 위에 재료가 없다면 재료를 조리도구 위로 올린다.
	else if (InteractActor->GetClass() == BP_CookingUtensil) {
		ACookingUtensil* CookingUtensil = Cast<ACookingUtensil>(InteractActor);
		if (!CookingUtensil->IsIngredientOn) {
			CookingUtensil->PutIngrOn(HoldingIngr);
		}
	}

	// 테이블 위에 아무것도 없다면 재료를 테이블 위로 올린다.
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

		// 들고 있던 것을 버린다.
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
