// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Views/STileView.h"

class UBaseItemAsset;
class UInventoryComponent;

struct FItemData
{
	TWeakObjectPtr<UBaseItemAsset> Item;
	int32 Count;
};


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

	TSharedPtr<STileView<TSharedPtr<FItemData>>> wItemsList;
	
	TWeakObjectPtr<UInventoryComponent> InventoryComponent;
	TArray<TSharedPtr<FItemData>> ItemsList;
	

	void OnInventoryUpdate();
	TSharedRef<ITableRow> GenerateItemTile(TSharedPtr<FItemData> InItem, const TSharedRef<STableViewBase>& InTable);
	TSharedRef<SWidget> GenerateItemRow(TWeakObjectPtr<UBaseItemAsset> InItem);
	
};
