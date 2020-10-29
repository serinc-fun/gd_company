// Fill out your copyright notice in the Description page of Project Settings.


#include "SInventoryContainer.h"
#include "SlateOptMacros.h"
#include "Brushes/SlateColorBrush.h"
#include "GD/Utilities.h"
#include "GD/Components/InventoryComponent.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Views/STileView.h"

class GD_API SInventoryItem : public SCompoundWidget
{
public:
	
	SLATE_BEGIN_ARGS(SInventoryItem)
	{}
	SLATE_ATTRIBUTE(TSharedPtr<FItemData>, Item)
	SLATE_END_ARGS()

    void Construct(const FArguments& InArgs)
	{
		Attr_Item = InArgs._Item;

		ChildSlot
		[
			SNew(SBox)
	        .WidthOverride(100)
	        .HeightOverride(100)
	        .Padding(8)
	        [
	            SNew(SOverlay)
	            + SOverlay::Slot()
	            [
	                SNew(SScaleBox)
	                .Stretch(EStretch::ScaleToFit)
	                [
	                    SNew(SImage)
	                    .Image(this, &SInventoryItem::GetImage)
	                ]
	            ]
	            + SOverlay::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
	            [
	                SNew(STextBlock)
	                .Text(this, &SInventoryItem::GetTitle)
	                .Font(FCoreStyle::GetDefaultFontStyle(NAME_None, 24))
	                .Justification(ETextJustify::Center)
	                .ColorAndOpacity(FSlateColor(FColorList::Orange))
	            ]
	            + SOverlay::Slot().HAlign(HAlign_Right).VAlign(VAlign_Bottom)
	            [
	            	SNew(SBorder)
	            	.BorderImage(new FSlateColorBrush(FLinearColor::Black))
	            	.Padding(0)
	            	[
	            		SNew(STextBlock)
		                .Text(this, &SInventoryItem::GetCount)
		                .Font(FCoreStyle::GetDefaultFontStyle(NAME_None, 24))
		                .ColorAndOpacity(FSlateColor(FColorList::Orange))
		                .Justification(ETextJustify::Center)
	            	]
	            ]
	        ]
		];
	}

protected:

	TAttribute<TSharedPtr<FItemData>> Attr_Item;

	const FSlateBrush* GetImage() const
	{
		if (Attr_Item.IsSet() && Attr_Item.Get().IsValid() && Attr_Item.Get()->Item.IsValid())
		{
			return &Attr_Item.Get()->Item->Brush;
		}

		static FSlateBrush* EmptyBrush = new FSlateNoResource();		
		return EmptyBrush;
	}

	FText GetTitle() const
	{
		if (Attr_Item.IsSet() && Attr_Item.Get().IsValid() && Attr_Item.Get()->Item.IsValid())
		{
			return Attr_Item.Get()->Item->Title;
		}

		return FText::GetEmpty();
	}

	FText GetCount() const
	{
		if (Attr_Item.IsSet() && Attr_Item.Get().IsValid() && Attr_Item.Get()->Count > 0)
		{
			return FText::AsNumber(Attr_Item.Get()->Count);
		}

		return FText::GetEmpty();
	}
};


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SInventoryContainer::Construct(const FArguments& InArgs, UInventoryComponent* InInventory)
{
	InventoryComponent = InInventory;
	
	ChildSlot.HAlign(HAlign_Left).VAlign(VAlign_Bottom)
	[
		SNew(SBox).WidthOverride(600).HeightOverride(350)
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
							SNew(SInventoryItem).Item_Lambda([&] { return SelectedItem; })
						]
					]
					+ SHorizontalBox::Slot()
					[
		                SNew(SButton)
		                .Text(NSLOCTEXT("Inventory", "Container.AddSelected", "Add Selected"))
		                .OnClicked(this, &SInventoryContainer::OnButtonClicked, true)
		                .HAlign(HAlign_Center).VAlign(VAlign_Center)
		            ]
					+ SHorizontalBox::Slot()
					[
						SNew(SButton)
						.Text(NSLOCTEXT("Inventory", "Container.RemoveItem", "Remove"))
						.OnClicked(this, &SInventoryContainer::OnButtonClicked, false)
						.HAlign(HAlign_Center).VAlign(VAlign_Center)
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
			SNew(SInventoryItem).Item(InItem)
		];
}

TSharedRef<SWidget> SInventoryContainer::GenerateItemRow(TWeakObjectPtr<UBaseItemAsset> InItem)
{
	return SNew(SInventoryItem).Item(MakeShareable(new FItemData({InItem, 0})));
}

FReply SInventoryContainer::OnButtonClicked(bool bAddButton)
{
	if (bAddButton)
	{
		if (SelectedItem.IsValid() && InventoryComponent.IsValid())
		{
			InventoryComponent->AddItem(SelectedItem->Item.Get());
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
	SelectedItem = MakeShareable(new FItemData({InItem, 0}));
}

void SInventoryContainer::OnInventorySelected(TSharedPtr<FItemData> InItem, ESelectInfo::Type SelectInfo)
{
	SelectedInventoryItem = InItem;
}
