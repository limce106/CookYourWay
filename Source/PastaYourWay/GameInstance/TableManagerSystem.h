// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TableManagerSystem.generated.h"

USTRUCT(BlueprintType)
struct FPlayerBistroData : public FTableRowBase {
	GENERATED_BODY()

public:
	FPlayerBistroData() : IsChain(0), PlayerLevel(1), PlayerAssets(500), PlayerBistroRating(2.5) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool IsChain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 PlayerLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 PlayerAssets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float PlayerBistroRating;
};

USTRUCT(BlueprintType)
struct FCustomerData : public FTableRowBase {
	GENERATED_BODY()

public:
	FCustomerData() : CustName("-1"), CustTasteCount(0), CustUnqTaste(0), RegBistro("-1"), CustRoyalty(-1), CustGroupCode(0), CustCreateProb(0), CustWaitingTime(0), CustIcon(nullptr), CustModel(nullptr) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString CustName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 CustTasteCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 CustUnqTaste;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString RegBistro;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 CustRoyalty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 CustGroupCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 CustCreateProb;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 CustWaitingTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UTexture2D> CustIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UMeshComponent> CustModel;
};

UCLASS()
class PASTAYOURWAY_API UTableManagerSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UTableManagerSystem();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Table")
	class UDataTable* PlayerBistroTable;
	class UDataTable* CustomerTable;

	TArray<FPlayerBistroData*> PlayerBistroTableRows;
	TArray<FCustomerData*> CustomerTableRows;
};
