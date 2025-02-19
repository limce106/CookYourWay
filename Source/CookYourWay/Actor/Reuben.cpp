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
	// �տ� �ƹ��͵� ��� ���� ������
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
	// ������ġ�� ���� �� �����̰ų� �մ��� �Դ� ���̶��
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
