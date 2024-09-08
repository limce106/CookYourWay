// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CustomerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class COOKYOURWAY_API UCustomerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

	class ACustomer* Customer;

public:
	UPROPERTY(BlueprintReadOnly)
	bool IsWalk;

	UPROPERTY(BlueprintReadOnly)
	UAnimSequence* WalkSeq;
	UPROPERTY(BlueprintReadOnly)
	UAnimSequence* IdleSeq;
};
