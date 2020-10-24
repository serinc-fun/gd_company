// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UInventoryComponent;
/**
 * 
 */
class GD_API SInventoryContainer : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SInventoryContainer)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UInventoryComponent* InInventory);

protected:

	TWeakObjectPtr<UInventoryComponent> InventoryComponent;

	void OnInventoryUpdate();
	
};
