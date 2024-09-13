// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_ArtifactRewardScreen.generated.h"


class UTextBlock;
class UUW_StatusIcon;
class UButton;
class UStatusComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReturnSelectionArtifact, bool, bSkipped, UStatusComponent*,Artifact);

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class CROWNOFSIN_API UUW_ArtifactRewardScreen : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_ArtifactRewardScreen(const FObjectInitializer& ObjectInitializer);


public:
	UFUNCTION(BlueprintCallable, Category = "ArtifactRewardScreen")
	void UpdateReward(UStatusComponent* Status);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Artifact", meta = (BindWidget))
	TObjectPtr<UButton> ArtifactButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Artifact", meta = (BindWidget))
	TObjectPtr<UButton> SkipButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Artifact", meta = (BindWidget))
	TObjectPtr<UTextBlock> Title;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Artifact", meta = (BindWidget))
	TObjectPtr<UUW_StatusIcon> WBP_StatusIcon;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Artiface")
	TObjectPtr<UStatusComponent> Artifact;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Artifact",Transient,meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn;
	
	// Delegate
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "ArtifactRewardScreen")
	FOnReturnSelectionArtifact OnReturnSelectionArtifact;
};
