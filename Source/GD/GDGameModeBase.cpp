// Copyright Epic Games, Inc. All Rights Reserved.


#include "GDGameModeBase.h"
#include "UI/SInventoryContainer.h"

AGDGameModeBase::AGDGameModeBase()
    : Super()
{
    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void AGDGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    auto targetWidget = SNew(SInventoryContainer, InventoryComponent);

    if (auto targetViewport = GetGameInstance()->GetGameViewportClient())
    {
        targetViewport->AddViewportWidgetContent(targetWidget);
    }  
   
}
