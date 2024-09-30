// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ReubenController.h"

AReubenController::AReubenController()
{
	Reuben = (AReuben*)GetOwner();
}

void AReubenController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this, &AReubenController::Interaction);
	InputComponent->BindAction("Chop", EInputEvent::IE_Pressed, Reuben, &AReuben::Chop);
}

void AReubenController::Interaction()
{
	AActor* OverlappedActor = Reuben->OverlappedActor;
	if (Reuben->IsHold == false) {
		Reuben->EmptyOnSocketInteraction(OverlappedActor);
	}
	else {
		TArray<USceneComponent*> HoldScoketComponents = Reuben->GetMesh()->GetAttachChildren();
		if (HoldScoketComponents[0]->GetClass() == Reuben->BP_Plate) {
			Reuben->PlateOnSocketInteraction(OverlappedActor);
		}
		else if (HoldScoketComponents[0]->GetClass() == Reuben->BP_CookingUtensil) {
			Reuben->CookingUtensilOnSocketInteraction(OverlappedActor);
		}
		else if (HoldScoketComponents[0]->GetClass() == Reuben->BP_Ingredient) {
			Reuben->IngrOnSocketInteraction(OverlappedActor);
		}
	}
}

//void AReubenController::PickUp()
//{
//	AActor* OverlappedActor = Reuben->OverlappedActor;
//
//	// 조리도구를 들려고 할 때
//	if (OverlappedActor->GetClass() == BP_CookingUtensil) {
//
//		if (Reuben->IsHold == true) {
//			return;
//		}
//
//		// 조리도구 위에 재료가 없으면 조리도구를 들고, 있다면 재료를 든다.
//		if (((ACookingUtensil*)OverlappedActor)->IsIngredientOn == false) {
//			OverlappedActor->AttachToComponent(Reuben->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HoldSocket"));
//		}
//		else {
//			((ACookingUtensil*)OverlappedActor)->PlacedIngredient->AttachToComponent(Reuben->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HoldSocket"));
//		}
//	}
//
//	// 재료를 들려고 할 때
//	else if (OverlappedActor->GetClass()->IsChildOf(BP_Ingredient)) {
//
//		if (Reuben->IsHold == true) {
//			TArray<USceneComponent*> HoldScoketComponents = Reuben->GetMesh()->GetAttachChildren();
//			// 접시를 들고 있을 때
//			if (HoldScoketComponents[0]->GetClass() == BP_Plate) {
//
//			}
//			else {
//				return;
//			}
//		}
//		else {
//			OverlappedActor->AttachToComponent(Reuben->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HoldSocket"));
//		}
//	}
//}


