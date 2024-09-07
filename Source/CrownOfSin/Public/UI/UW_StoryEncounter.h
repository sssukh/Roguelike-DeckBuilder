// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/CosEnumStruct.h"
#include "Interfaces/Interface_EventHolder.h"
#include "Interfaces/Interface_StoryEncounter.h"
#include "UW_StoryEncounter.generated.h"

class UUW_StoryButton;
class URichTextBlock;
class UVerticalBox;
class UImage;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_StoryEncounter : public UUserWidget, public IInterface_EventHolder, public IInterface_StoryEncounter
{
	GENERATED_BODY()
public:
	UUW_StoryEncounter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SetupStoryEncounter(const FText& InDescription, UTexture2D* Image, TArray<FCard> EncounterOptions, bool bIsFirstScreen);

	UFUNCTION(BlueprintCallable)
	void BindRemovalToEmptyButton(UUW_StoryButton* StoryButton);
	
	UFUNCTION(BlueprintCallable)
	void DisableOptions();

	UFUNCTION(BlueprintCallable)
	void RemoveOnEmptyButtonClicked(UUserWidget* CallingWidget);

	virtual void NativeDestruct() override;

	// Interface_EventHolder
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

	// Interface_StoryEncounter
	virtual void InitializeStoryEncounter_Implementation(FDataTableRowHandle EncounterData, bool bIsFirstScreen) override;
	// Variables
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Story Encounter Widget",meta=(BindWidget))
	TObjectPtr<UImage> Background;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Story Encounter Widget",meta=(BindWidget))
	TObjectPtr<UVerticalBox> ButtonBox;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Story Encounter Widget",meta=(BindWidget))
	TObjectPtr<URichTextBlock> Description;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Story Encounter Widget",meta=(BindWidget))
	TObjectPtr<UVerticalBox> StoryBox;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Story Encounter Widget",meta=(BindWidget))
	TArray<UUW_StoryButton*> StoryButtons;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Story Encounter Widget | Config")
	TSubclassOf<UUW_StoryButton> StoryButtonClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Transient, meta=(BindWidgetAnim),Category="Story Encounter Widget")
	TObjectPtr<UWidgetAnimation> StoryBoxFadeIn;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Transient, meta=(BindWidgetAnim),Category="Story Encounter Widget")
	TObjectPtr<UWidgetAnimation> FadeIn;

};
