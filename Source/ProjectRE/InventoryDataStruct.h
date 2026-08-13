#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "InventoryDataStruct.generated.h"

class UItemDataAsset;

USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_BODY()

UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UItemDataAsset* ItemDataAsset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemQuantity;
};
