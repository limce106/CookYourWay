// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PreviewSandwich.h"
#include "EngineUtils.h"

APreviewSandwich::APreviewSandwich()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
}

void APreviewSandwich::BeginPlay()
{
	Super::BeginPlay();

	CaptureComp = PreviewCaptureActor->GetCaptureComponent2D();
	CaptureComp->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	CaptureComp->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
	
	CaptureComp->ShowOnlyActors.Empty();
	CaptureComp->ShowOnlyActors.Add(this);

	CaptureComp->ShowFlags.SetStaticMeshes(true);

	if (CaptureComp->TextureTarget)
	{
		CaptureComp->TextureTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
		CaptureComp->TextureTarget->ClearColor = FLinearColor::White;
	}
	CaptureComp->CaptureScene();
}

void APreviewSandwich::SetIngredients(const TArray<AActor*>& SourceIngredients)
{
	float ZOffset = 0.0f;

	// 기존 구성 제거
	for (UStaticMeshComponent* Comp : PreviewComponents)
	{
		if (Comp)
			Comp->DestroyComponent();
	}
	PreviewComponents.Empty();

	for (AActor* Ingr : SourceIngredients)
	{
		if (!Ingr) continue;

		UStaticMeshComponent* SoureMesh = Ingr->FindComponentByClass<UStaticMeshComponent>();
		if (!SoureMesh || !SoureMesh->GetStaticMesh()) continue;

		// 새로운 재료 스태틱 메시 생성
		UStaticMeshComponent* NewMesh = NewObject<UStaticMeshComponent>(this);
		NewMesh->SetStaticMesh(SoureMesh->GetStaticMesh());
		NewMesh->SetRelativeScale3D(SoureMesh->GetComponentScale());

		NewMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

		// 그림자 끄기
		NewMesh->SetCastShadow(false);
		NewMesh->bCastDynamicShadow = false;
		NewMesh->bCastStaticShadow = false;

		// 월드에 컴포넌트 추가
		NewMesh->RegisterComponent();

		// 메시 피봇 중앙 맞추기
		FVector MinBound, MaxBound;
		NewMesh->GetLocalBounds(MinBound, MaxBound);
		FVector MeshCenter = ((MinBound + MaxBound) / 2.0f) * NewMesh->GetComponentScale();

		NewMesh->SetRelativeLocation(FVector(0.0f, 0.0f, ZOffset) - MeshCenter);
		ZOffset += (MaxBound.Z - MinBound.Z) * NewMesh->GetComponentScale().Z + 2.0f;

		PreviewComponents.Add(NewMesh);
	}

	if (CaptureComp)
	{
		CaptureComp->ShowOnlyComponents.Empty();

		for (UStaticMeshComponent* Comp : PreviewComponents)
		{
			if (Comp)
			{
				CaptureComp->ShowOnlyComponents.Add(Comp);
			}
		}

		CaptureComp->CaptureScene();
	}
}
