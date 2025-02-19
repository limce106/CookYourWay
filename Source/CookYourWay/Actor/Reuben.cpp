// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Reuben.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Subsystems/PanelExtensionSubsystem.h>
#include <Kismet/GameplayStatics.h>
#include "PlayerBistro.h"
#include <Components/ShapeComponent.h>
#include "Dessert.h"
#include "FryPan.h"

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

void AReuben::UpdateClosestOverlappingActor()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	float ClosestDistance = 999;
	AActor* ClosestActor = nullptr;

	for (auto Actor : OverlappingActors) {
		float Distance = GetDistanceTo(Actor);
		if (ClosestDistance > Distance) {
			ClosestDistance = Distance;
			ClosestActor = Actor;
		}
	}

	if (ClosestActor != nullptr) {
		OverlappedActor = ClosestActor;
	}
}

UClass* AReuben::GetHeldActorClass()
{
	return (HeldActor->GetClass());
}

void AReuben::HoldActor(AActor* Actor)
{
	UShapeComponent* ActorCollision = Cast<UShapeComponent>(Actor->FindComponentByClass(UShapeComponent::StaticClass()));
	if (ActorCollision) {
		ActorCollision->SetCollisionProfileName(TEXT("NoCollision"));
	}

	HeldActor = Actor;
	IsHold = true;

	FRotator HeldActorRotation = GetActorRotation();
	HeldActorRotation.Yaw -= 180.0f;
	HeldActor->SetActorRotation(HeldActorRotation);
}

void AReuben::PutDownActor()
{
	UShapeComponent* ActorCollision = Cast<UShapeComponent>(HeldActor->FindComponentByClass(UShapeComponent::StaticClass()));
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

	InputComponent->BindAction("Chop", EInputEvent::IE_Pressed, this, &AReuben::Chop);
}


void AReuben::Chop()
{
	ACuttingBoard* CuttingBoard = Cast<ACuttingBoard>(UGameplayStatics::GetActorOfClass(GetWorld(), BP_CuttingBoard));
	if (IsOverlappingActor(CuttingBoard)) {
		CuttingBoard->Chop();
	}
}

bool AReuben::TryGiveSomething(ACustomer* Customer)
{
	// 손에 아무것도 들고 있지 않으면
	if (IsHold == false) {
		return false;
	}

	if (HeldActor->GetClass() == BP_Sandwich) {
		GiveSandwich(Customer);
		return true;
	}
	else if (HeldActor->GetClass() == BP_Dessert) {
		ADessert* Dessert = Cast<ADessert>(HeldActor);
		if (Dessert->IsCooked && Customer->CanGetDessert()) {
			GiveDessert(Customer);
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

void AReuben::GiveSandwich(ACustomer* Customer)
{
	GivenSandwichNum++;
	ASandwich* Sandwich = Cast<ASandwich>(HeldActor);
	// 샌드위치가 없는 빈 접시이거나 손님이 먹는 중이라면
	if (Sandwich->Ingredients.Num() == 0 || Customer->IsEat) {
		return;
	}
	Sandwich->SetIngrWidgetVisibility(ESlateVisibility::Hidden);

	ADiningTable* DiningTable = PlayerBistro->GetDiningTable(Customer->CurSeatNum);
	DiningTable->PutFoodOn(this, Sandwich);
	Customer->AddPlayerSandwichReview(Sandwich);

	Customer->EatSandwich();
}

void AReuben::GiveDessert(ACustomer* Customer)
{
	ADiningTable* DiningTable = PlayerBistro->GetDiningTable(Customer->CurSeatNum);
	DiningTable->PutFoodOn(this, HeldActor);

	Customer->EatDessert();
	Customer->AddDessertReview();
}
