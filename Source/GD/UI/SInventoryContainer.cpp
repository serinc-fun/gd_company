// Fill out your copyright notice in the Description page of Project Settings.


#include "SInventoryContainer.h"
#include "SlateOptMacros.h"
#include "GD/Utilities.h"
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
					SAssignNew(wSelector, SComboBox<TWeakObjectPtr<UBaseItemAsset>>)
					.OptionsSource(&SelectorList)
					.OnGenerateWidget(this, &SInventoryContainer::GenerateItemRow)
					.OnSelectionChanged(this, &SInventoryContainer::OnSelectorSelected)
					.Content()
					[
						SNew(STextBlock)
						.Text_Lambda([&] { return SelectedItem.IsValid() ? SelectedItem->Title : FText::FromString("NULL"); })
					]
				]
				+ SHorizontalBox::Slot()
				[
	                SNew(SButton)
	                .Text(NSLOCTEXT("Inventory", "Container.AddSelected", "Add Selected"))
	                .OnClicked(this, &SInventoryContainer::OnButtonClicked, true)
	            ]
				+ SHorizontalBox::Slot()
				[
					SNew(SButton)
					.Text(NSLOCTEXT("Inventory", "Container.RemoveItem", "Remove"))
					.OnClicked(this, &SInventoryContainer::OnButtonClicked, false)
				]
			]
		]
		+ SVerticalBox::Slot().Padding(4)
		[
			SAssignNew(wItemsList, STileView<TSharedPtr<FItemData>>)
			.SelectionMode(ESelectionMode::Single)
			.ListItemsSource(&ItemsList)
			.OnGenerateTile(this, &SInventoryContainer::GenerateItemTile)
			.OnSelectionChanged(this, &SInventoryContainer::OnInventorySelected)
		]
	];

	if (InventoryComponent.IsValid())
	{
		InventoryComponent->OnInventoryUpdate.AddSP(this, &SInventoryContainer::OnInventoryUpdate);
		
		TArray<FAssetData> OutAssets;
		if (UUtilities::GetAllAssetData(UBaseItemAsset::StaticClass(), OutAssets))
		{			
			for (auto asset : OutAssets)
			{
				if (auto castedAsset = Cast<UBaseItemAsset>(asset.GetAsset()))
				{
					SelectorList.Add(castedAsset);
				}
			}
		}

		wSelector->RefreshOptions();
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

FReply SInventoryContainer::OnButtonClicked(bool bAddButton)
{
	if (bAddButton)
	{
		if (SelectedItem.IsValid() && InventoryComponent.IsValid())
		{
			InventoryComponent->AddItem(SelectedItem.Get());
		}
	}
	else
	{
		if (SelectedInventoryItem.IsValid() && InventoryComponent.IsValid())
		{
			InventoryComponent->RemoveItem(SelectedInventoryItem->Item.Get());
		}
	}
	
	return FReply::Handled();
}

void SInventoryContainer::OnSelectorSelected(TWeakObjectPtr<UBaseItemAsset> InItem, ESelectInfo::Type SelectInfo)
{
	SelectedItem = InItem;
}

void SInventoryContainer::OnInventorySelected(TSharedPtr<FItemData> InItem, ESelectInfo::Type SelectInfo)
{
	SelectedInventoryItem = InItem;
}
