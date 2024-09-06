// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/CosEnumStruct.h"
#include "UW_StoryButton.generated.h"

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_StoryButton : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_StoryButton(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	FCard EncounterCard;
	
};
