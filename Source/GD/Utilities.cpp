// Copyright Epic Games, Inc. All Rights Reserved.

#include "Utilities.h"
#include "AssetRegistryModule.h"

bool UUtilities::GetAllAssetData(UClass* BaseClass, TArray<FAssetData>& AssetList)
{
    static FName RegistryModuleName = TEXT("AssetRegistry");

    if (BaseClass == UBlueprint::StaticClass())
    {
        UE_LOG(LogInit, Error, TEXT("GetAllAssetData() should not be used for blueprints; call GetAllBlueprintAssetData() instead"));
        return false;
    }

    FAssetRegistryModule* AssetRegistryModule = nullptr;
    if (FModuleManager::Get().IsModuleLoaded(RegistryModuleName))
    {
        AssetRegistryModule = (FAssetRegistryModule*)FModuleManager::Get().GetModule(RegistryModuleName);
    }
    else
    {
        FModuleStatus ModuleStatus;
        if (FModuleManager::Get().QueryModule(RegistryModuleName, ModuleStatus) == false)
        {
            AssetRegistryModule = &FModuleManager::LoadModuleChecked<FAssetRegistryModule>(RegistryModuleName);
        }
    }

    if (AssetRegistryModule)
    {
        IAssetRegistry& AssetRegistry = AssetRegistryModule->Get();

        AssetRegistry.ScanPathsSynchronous({
            TEXT("/Game"),
            });
        FARFilter ARFilter;
        if (BaseClass != nullptr)
        {
            ARFilter.ClassNames.Add(BaseClass->GetFName());
            TArray<FName> OldNames = FLinkerLoad::FindPreviousNamesForClass(BaseClass->GetPathName(), false);
            ARFilter.ClassNames.Append(OldNames);
        }
        ARFilter.bRecursivePaths = true;
        ARFilter.bIncludeOnlyOnDiskAssets = true;
        ARFilter.bRecursiveClasses = true;
        AssetRegistry.GetAssets(ARFilter, AssetList);

        return AssetList.Num() > 0;
    }

    return false;
}
