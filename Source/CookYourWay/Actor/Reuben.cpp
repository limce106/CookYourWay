// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Reuben.h"
#include "GameFramework/CharacterMovementComponent.h"
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

	HoldingComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HoldingComponent"));
	HoldingComponent->SetupAttachment(GetRootComponent());
	HoldingComponent->SetRelativeLocation(FVector(50.0f, 0.0f, 25.0f));
}

void AReuben::BeginPlay()
{
	Super::BeginPlay();

}

void AReuben::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//SetHeldActorLoc();
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

void AReuben::UpdateOverlappingActor()
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
	if (HeldActor) return;

	UPrimitiveComponent* ActorCollision = Cast<UPrimitiveComponent>(Actor->FindComponentByClass(UShapeComponent::StaticClass()));
	if (ActorCollision) {
		ActorCollision->SetCollisionProfileName(TEXT("NoCollision"));
	}

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false);
	Actor->AttachToComponent(HoldingComponent, AttachmentRules);

	HeldActor = Actor;
	IsHold = true;

	/*FRotator HeldActorRotation = GetActorRotation();
	HeldActorRotation.Yaw -= 180.0f;
	HeldActor->SetActorRotation(HeldActorRotation);*/
}

void AReuben::PutDownActor()
{
	if (!HeldActor) return;

	HeldActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	UShapeComponent* ActorCollision = Cast<UShapeComponent>(HeldActor->FindComponentByClass(UShapeComponent::StaticClass()));
	if (ActorCollision) {
		if ((HeldActor->GetClass() == BP_Ingredient) || (HeldActor->GetClass() == BP_Sandwich)) {
			ActorCollision->SetCollisionProfileName(TEXT("OnSomething"));
			USoundBase* LoadedSound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Assets/Sound/SoundAsset/SFX_PutIngredients.SFX_PutIngredients"));
			UGameplayStatics::PlaySoundAtLocation(this, LoadedSound, GetActorLocation());
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
	if (OverlappedActor->GetClass()->IsChildOf(ACuttingBoard::StaticClass())) {
		ACuttingBoard* CuttingBoard = Cast<ACuttingBoard>(OverlappedActor);
		CuttingBoard->Chop();
	}
}
