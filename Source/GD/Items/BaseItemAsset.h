// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BaseItemAsset.generated.h"

/**
 * 
 */
UCLASS()
class GD_API UBaseItemAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UBaseItemAsset();

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	FText Title;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
	FSlateBrush Brush;
};
