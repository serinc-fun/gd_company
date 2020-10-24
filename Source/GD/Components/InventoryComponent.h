// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GD/Items/BaseItemAsset.h"
#include "InventoryComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnInventoryAction);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GD_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UInventoryComponent();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	int32 AddItem(UBaseItemAsset* InItem);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void AddItems(TArray<UBaseItemAsset*> InItems);

	FOnInventoryAction OnInventoryUpdate;
	
	UPROPERTY(Transient)
	TMap<UBaseItemAsset*, int32> Inventory;
		
};
