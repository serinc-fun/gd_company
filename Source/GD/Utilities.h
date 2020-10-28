// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Utilities.generated.h"

UCLASS(MinimalAPI)
class UUtilities : public UObject
{
    GENERATED_BODY()

public:

    static bool GetAllAssetData(UClass* BaseClass, TArray<FAssetData>& AssetList);
};
