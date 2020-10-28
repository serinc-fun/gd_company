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
		SNew(SVerticalBox)
		+ SVerticalBox::Slot().AutoHeight()
		[
			SNew(SBox)
			.HeightOverride(120).Padding(4)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SComboBox<TWeakObjectPtr<UBaseItemAsset>>)
					.OnGenerateWidget(this, &SInventoryContainer::GenerateItemRow)
				]
				+ SHorizontalBox::Slot()
				[
	                SNew(SButton)
	                .Text(NSLOCTEXT("Inventory", "Container.AddSelected", "Add Selected"))
	            ]
				+ SHorizontalBox::Slot()
				[
					SNew(SButton)
					.Text(NSLOCTEXT("Inventory", "Container.RemoveItem", "Remove"))
				]
			]
		]
		+ SVerticalBox::Slot().Padding(4)
		[
			SAssignNew(wItemsList, STileView<TSharedPtr<FItemData>>)
			.SelectionMode(ESelectionMode::Single)
			.ListItemsSource(&ItemsList)
			.OnGenerateTile(this, &SInventoryContainer::GenerateItemTile)
		]
	];

	if (InventoryComponent.IsValid())
	{
		InventoryComponent->OnInventoryUpdate.AddSP(this, &SInventoryContainer::OnInventoryUpdate);
	}
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SInventoryContainer::OnInventoryUpdate()
{
	ItemsList.Empty();
	
	if (InventoryComponent.IsValid())
	{
		for (auto pair : InventoryComponent->Inventory)
		{
			TSharedRef<FItemData> item = MakeShareable(new FItemData);
			item->Item = pair.Key;
			item->Count = pair.Value;
			
			ItemsList.Add(item);
		}
	}

	wItemsList->RequestListRefresh();
}

TSharedRef<ITableRow> SInventoryContainer::GenerateItemTile(TSharedPtr<FItemData> InItem, const TSharedRef<STableViewBase>& InTable)
{
	return SNew(STableRow<TSharedPtr<FItemData>>, InTable)
		[
			SNew(SBox)
			.WidthOverride(100)
			.HeightOverride(100)
			[
				SNew(STextBlock)
				.Text(InItem->Item->Title)
			]
		];
}

TSharedRef<SWidget> SInventoryContainer::GenerateItemRow(TWeakObjectPtr<UBaseItemAsset> InItem)
{
	return SNew(SBox)
            .WidthOverride(100)
            .HeightOverride(100)
            [
                SNew(STextBlock)
                .Text(InItem->Title)
            ];
}
