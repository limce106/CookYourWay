// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ReubenController.h"

AReubenController::AReubenController()
{
	Reuben = (AReuben*)GetOwner();

	bShowMouseCursor = true;
}

void AReubenController::SetupInputComponent()
{
	Super::SetupInputComponent();

}

//void AReubenController::PickUp()
//{
//	AActor* OverlappedActor = Reuben->OverlappedActor;
//
//	// ���������� ����� �� ��
//	if (OverlappedActor->GetClass() == BP_CookingUtensil) {
//
//		if (Reuben->IsHold == true) {
//			return;
//		}
//
//		// �������� ���� ��ᰡ ������ ���������� ���, �ִٸ� ��Ḧ ���.
//		if (((ACookingUtensil*)OverlappedActor)->IsIngredientOn == false) {
//			OverlappedActor->AttachToComponent(Reuben->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HoldSocket"));
//		}
//		else {
//			((ACookingUtensil*)OverlappedActor)->PlacedIngredient->AttachToComponent(Reuben->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("HoldSocket"));
//		}
//	}
//
//	// ��Ḧ ����� �� ��
//	else if (OverlappedActor->GetClass()->IsChildOf(BP_Ingredient)) {
//
//		if (Reuben->IsHold == true) {
//			TArray<USceneComponent*> HoldScoketComponents = Reuben->GetMesh()->GetAttachChildren();
//			// ���ø� ��� ���� ��
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


