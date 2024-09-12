// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CustomerAnimInstance.h"
#include "Customer.h"

void UCustomerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Customer = Cast<ACustomer>(GetOwningActor());

	if (Customer) {
		FString WalkSeqPath = FString("/Game/Assets/Art_3D/Animation/").Append(Customer->CustName).Append("_Walk.").Append(Customer->CustName).Append("_Walk");
		FString IdleSeqPath = FString("/Game/Assets/Art_3D/Animation/").Append(Customer->CustName).Append("_Idle.").Append(Customer->CustName).Append("_Idle");
		UAnimSequence* Anim_WalkSeq = LoadObject<UAnimSequence>(NULL, *WalkSeqPath, NULL, LOAD_None, NULL);
		UAnimSequence* Anim_IdleSeq = LoadObject<UAnimSequence>(NULL, *IdleSeqPath, NULL, LOAD_None, NULL);
		WalkSeq = Anim_WalkSeq;
		IdleSeq = Anim_IdleSeq;
	}
}

void UCustomerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Customer) {
		IsWalk = Customer->IsWalk;
	}
}
