// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CustomerAnimInstance.h"
#include "Customer.h"

void UCustomerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Customer = Cast<ACustomer>(GetOwningActor());

	if (Customer) {
		FString WalkSeqPath = (TEXT("/Game/Assets/Art_3D/Animation/%s_Walk.%s_Walk"), Customer->CustName, Customer->CustName);
		FString IdleSeqPath = (TEXT("/Game/Assets/Art_3D/Animation/%s_Idle.%s_Idle"), Customer->CustName, Customer->CustName);
		const ConstructorHelpers::FObjectFinder<UAnimSequence> Anim_WalkSeq(*WalkSeqPath);
		const ConstructorHelpers::FObjectFinder<UAnimSequence> Anim_IdleSeq(*IdleSeqPath);
		WalkSeq = Anim_WalkSeq.Object;
		IdleSeq = Anim_IdleSeq.Object;
	}
}

void UCustomerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Customer) {
		IsWalk = Customer->IsWalk;
	}
}
