// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/InventoryComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GDGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GD_API AGDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AGDGameModeBase();

	virtual void BeginPlay() override;

protected:

	UPROPERTY(VisibleAnywhere, Category = Components)
	UInventoryComponent* InventoryComponent;
};
