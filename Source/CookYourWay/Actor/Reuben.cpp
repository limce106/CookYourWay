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

	SetHeldActorLoc();
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
	// 접시나 조리도구라면 든다.
	if (InteractActor->GetClass() == BP_Sandwich || InteractActor->GetClass()->IsChildOf(ACookingUtensil::StaticClass())) {
		HoldActor(InteractActor);
	}

	// 타지 않은 조리된 재료라면 든다.
	else if (InteractActor->GetClass() == BP_Ingredient) {
		AIngredient* Ingredient = Cast<AIngredient>(InteractActor);
		if (Ingredient->IsCooked() && !Ingredient->IsBurn) {
			HoldActor(Ingredient);
		}
	}

	// 접시를 스폰하여 플레이어가 들게 한다.
	else if (InteractActor->GetClass() == BP_Plates) {
		ASandwich* Sandwich = GetWorld()->SpawnActor<ASandwich>(BP_Sandwich, GetActorLocation(), GetActorRotation());
		HoldActor(Sandwich);
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
	ASandwich* HoldingSandwich = Cast<ASandwich>(HeldActor);

	// 타지 않은, 조리된 재료라면 접시/샌드위치 위로 올린다.
	if (InteractActor->GetClass() == BP_Ingredient) {
		AIngredient* Ingredient = Cast<AIngredient>(InteractActor);
		if (Ingredient->IsCooked() && !Ingredient->IsBurn) {
			HoldingSandwich->AddIngredient(Ingredient);
		}
	}

	// 타지 않은, 조리된 재료가 조리도구 위에 있다면 접시/샌드위치 위로 올린다.
	else if (InteractActor->GetClass()->IsChildOf(ACookingUtensil::StaticClass())) {
		ACookingUtensil* CookingUtensil = Cast<ACookingUtensil>(InteractActor);
		if (CookingUtensil->PlacedIngredient->IsCooked() && !CookingUtensil->PlacedIngredient->IsBurn) {
			HoldingSandwich->AddIngredient(CookingUtensil->PlacedIngredient);
		}
	}

	// 테이블 위에 아무것도 없다면 접시/샌드위치를 테이블 위로 올린다.
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
	AIngredient* HoldingIngr = Cast<AIngredient>(HeldActor);

	// 타지 않은, 조리된 재료라면 접시/샌드위치 위로 올린다.
	if (InteractActor->GetClass() == BP_Sandwich) {
		ASandwich* Sandwich = Cast<ASandwich>(InteractActor);
		if (HoldingIngr->IsCooked() && !HoldingIngr->IsBurn) {
			Sandwich->AddIngredient(HoldingIngr);
		}
	}

	// 조리도구 위에 재료가 없다면 재료를 조리도구 위로 올린다.
	else if (InteractActor->GetClass()->IsChildOf(ACookingUtensil::StaticClass())) {
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
	if (!OverlappedActor || !IsOverlappingActor(OverlappedActor)) {
		return;
	}

	// 손님
	if (OverlappedActor->GetClass() == BP_Customer) {
		ACustomer* Customer = Cast<ACustomer>(OverlappedActor);
		TryGiveSomething(Customer);
	}
	// 냉장고
	else if (OverlappedActor->GetClass() == BP_Fridge) {
		UIngredientBoardWidget* BP_IngredientBoard = CreateWidget<UIngredientBoardWidget>(GetWorld(), BP_IngredientBoardClass);
		if (BP_IngredientBoard) {
			BP_IngredientBoard->AddToViewport();
		}
		return;
	}

	// 손에 아무것도 안 들고 있을 때
	if (IsHold == false) {
		EmptyOnSocketInteraction(OverlappedActor);
	}
	// 손에 무언가 들고 있을 때
	else {
		// 손에 들고 있던 것을 버린다.
		if (OverlappedActor->GetClass() == BP_TrashBin) {
			HeldActor->Destroy();
			IsHold = false;
			return;
		}

		// 손에 들고 있는 것이 샌드위치
		if (HeldActor->GetClass() == BP_Sandwich) {
			SandwichOnSocketInteraction(OverlappedActor);
		}
		// 손에 들고 있는 것이 조리도구
		else if (HeldActor->GetClass()->IsChildOf(ACookingUtensil::StaticClass())) {
			CookingUtensilOnSocketInteraction(OverlappedActor);
		}
		// 손에 들고 있는 것이 재료
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
	// 손에 아무것도 들고 있지 않으면
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
	// 샌드위치가 없는 빈 접시라면 
	if (Sandwich->Ingredients.Num() == 0) {
		return;
	}

	Customer->AddSandwichReview(Sandwich);
	Sandwich->Destroy();
	IsHold = false;

	// 손님 제거 타이머 시작
	Customer->StartDestroyTimer();
}

void AReuben::GiveDessert(ACustomer* Customer)
{
	// 손님이 디저트를 받을 수 있다면 (일정 식사시간이 지났다면)
	if (Customer->CanGetDessert()) {
		HeldActor->Destroy();
		IsHold = false;
		Customer->AddDessertReview();
	}
}
