// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CustomerAnimInstance.h"
#include "Customer.h"

void UCustomerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Customer = Cast<ACustomer>(GetOwningActor());

	if (Customer) {
		FString CustomerName = GetClass()->GetName();
		CustomerName.RemoveFromStart(TEXT("_AnimBP"));
		FString WalkSeqPath = (TEXT("/Game/Assets/Art_3D/Animation/%s_Walk.%s_Walk"), CustomerName);
		FString IdleSeqPath = (TEXT("/Game/Assets/Art_3D/Animation/%s_Idle.%s_Idle"), CustomerName);
		WalkSeq = LoadObject<UAnimSequence>(NULL, *WalkSeqPath);
		IdleSeq = LoadObject<UAnimSequence>(NULL, *IdleSeqPath);
	}
}

void UCustomerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Customer) {
		IsWalk = Customer->IsWalk;
	}
}
