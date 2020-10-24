// Fill out your copyright notice in the Description page of Project Settings.


#include "SInventoryContainer.h"
#include "SlateOptMacros.h"
#include "GD/Components/InventoryComponent.h"
#include "Widgets/Views/STileView.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SInventoryContainer::Construct(const FArguments& InArgs, UInventoryComponent* InInventory)
{
	InventoryComponent = InInventory;
	
	ChildSlot
	[
		SNew(STileView<TWeakObjectPtr<UObject>>)
		.SelectionMode(ESelectionMode::Single)
	];

	if (InventoryComponent.IsValid())
	{
		InventoryComponent->OnInventoryUpdate.AddSP(this, &SInventoryContainer::OnInventoryUpdate);
	}
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SInventoryContainer::OnInventoryUpdate()
{
	TArray<TPair<UBaseItemAsset*, int32>> Items;

	if (InventoryComponent.IsValid())
	{
		for (auto pair : InventoryComponent->Inventory)
		{
			Items.Add(pair);
		}
	}
}