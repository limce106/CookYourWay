// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/SceneCapture2D.h>
#include <Components/SceneCaptureComponent2D.h>
#include "Engine/TextureRenderTarget2D.h"
#include "PreviewSandwich.generated.h"

UCLASS()
class COOKYOURWAY_API APreviewSandwich : public AActor
{
	GENERATED_BODY()

	
public:	
	APreviewSandwich();
	void SetIngredients(const TArray<AActor*>& SourceIngredients);

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY()
	TArray<UStaticMeshComponent*> PreviewComponents;

	UPROPERTY(EditInstanceOnly)
	ASceneCapture2D* PreviewCaptureActor;
	UPROPERTY()
	USceneCaptureComponent2D* CaptureComp;
};
