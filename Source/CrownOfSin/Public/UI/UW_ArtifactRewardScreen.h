// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ArtifactRewardScreen.generated.h"


class UStatusComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReturnSelectionArtifact, bool, bSkipped, UStatusComponent*,Artifact);

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_ArtifactRewardScreen : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_ArtifactRewardScreen(const FObjectInitializer& ObjectInitializer);
	
	// Delegate
	
public:
	FOnReturnSelectionArtifact OnReturnSelectionArtifact;
};
