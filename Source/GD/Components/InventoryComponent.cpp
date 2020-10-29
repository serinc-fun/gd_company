// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{

}

int32 UInventoryComponent::AddItem(UBaseItemAsset* InItem)
{
    int32& Value = Inventory.FindOrAdd(InItem, 0);
    ++Value;
    OnInventoryUpdate.Broadcast();
    return Value;
}

void UInventoryComponent::AddItems(TArray<UBaseItemAsset*> InItems)
{
    for (auto item : InItems)
    {
        ++Inventory.FindOrAdd(item, 0);
    }

    OnInventoryUpdate.Broadcast();
}

bool UInventoryComponent::RemoveItem(UBaseItemAsset* InItem, int32 InAmountToRemove)
{
    if (Inventory.Contains(InItem))
    {
        auto& item = Inventory.FindChecked(InItem);
        item -= InAmountToRemove;

        if (item <= 0)
        {
            Inventory.Remove(InItem);
        }

        OnInventoryUpdate.Broadcast();        
        return true;
    }

    return false;
}
