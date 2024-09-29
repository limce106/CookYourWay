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
	InputComponent->BindAction("Chop", EInputEvent::IE_Pressed, this, &AReubenController::Chop);
}

void AReubenController::Interaction()
{
	AActor* OverlappedActor = Reuben->OverlappedActor;
	if (Reuben->IsHold == false) {
		EmptyOnSocketInteraction(OverlappedActor);
	}
	else {
		TArray<USceneComponent*> HoldScoketComponents = Reuben->GetMesh()->GetAttachChildren();
		if (HoldScoketComponents[0]->GetClass() == BP_Plate) {
			PlateOnSocketInteraction(OverlappedActor);
		}
		else if (HoldScoketComponents[0]->GetClass() == BP_CookingUtensil) {
			CookingUtensilOnSocketInteraction(OverlappedActor);
		}
		else if (HoldScoketComponents[0]->GetClass() == BP_Ingredient) {
			IngrOnSocketInteraction(OverlappedActor);
		}
	}
}

void AReubenController::Chop()
{
	BP_Chop();
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

void AReubenController::EmptyOnSocketInteraction(AActor* InteractActor)
{
	// 접시나 조리도구라면 든다.
	if (InteractActor->GetClass() == BP_Plate ||  InteractActor->GetClass() == BP_CookingUtensil) {

		InteractActor->AttachToComponent(Reuben->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HoldSocket"));
	}
	// 조리된 재료라면 든다.
	else if (InteractActor->GetClass() == BP_Ingredient) {

	}
}

void AReubenController::PlateOnSocketInteraction(AActor* InteractActor)
{
	// 조리된 재료라면 접시 위로 올린다.
	if (InteractActor->GetClass() == BP_Ingredient) {

	}
	// 조리된 재료가 조리도구 위에 있다면 접시 위로 올린다.
	else if (InteractActor->GetClass() == BP_CookingUtensil) {

	}
	// 테이블 위에 아무것도 없다면 접시를 테이블 위로 올린다.
	else if (InteractActor->GetClass() == BP_Table) {

	}
}

void AReubenController::CookingUtensilOnSocketInteraction(AActor* InteractActor)
{
	// 조리도구 위에 재료가 없다면 재료를 조리도구 위로 올린다.
	if (InteractActor->GetClass() == BP_Ingredient) {

	}
	// 조리도구 위에 조리된 재료가 있다면 재료를 접시 위로 올린다.
	else if (InteractActor->GetClass() == BP_Plate) {

	}
	// 테이블 위에 아무것도 없다면 조리도구를 테이블 위로 올린다.
	else if (InteractActor->GetClass() == BP_Table) {

	}
}

void AReubenController::IngrOnSocketInteraction(AActor* InteractActor)
{
	// 조리된 재료라면 접시 위로 올린다.
	if (InteractActor->GetClass() == BP_Plate) {

	}
	// 재료를 조리도구 위로 올린다.
	else if (InteractActor->GetClass() == BP_CookingUtensil) {

	}
	// 테이블 위에 아무것도 없다면 재료를 테이블 위로 올린다.
	else if(InteractActor->GetClass() == BP_Table) {

	}
}
