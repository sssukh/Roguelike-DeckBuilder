// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Interface_StoryEncounter.h"
#include "UW_Layout_Cos.generated.h"

class UUW_Shop;
class UButton;
class UUW_ScreenFade;
class UWidgetSwitcher;
class USizeBox;
class UUW_RewardScreen;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_Layout_Cos : public UUserWidget, public IInterface_StoryEncounter
{
	GENERATED_BODY()

public:
	UUW_Layout_Cos(const FObjectInitializer& ObjectInitializer);


	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category="UW Layout")
	void SetupDebugMode();

	UFUNCTION(BlueprintCallable, Category="UW Layout")
	void UpdateHandAnchorWidgets();

public:
	UFUNCTION(BlueprintCallable, Category="UW Layout Event")
	bool ShouldNodeMapBeBlocked();


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="UW Layout")
	void DisplayScreenLogMessage(const FText& Message, const FColor& Color);

	/*ToDo:구현해야합니다.*/
	void UpdatePileWidgetAmount(FGameplayTag PileTag, int32 NewAmount)
	{
	}

	/*========================================================================================
	*	Iinterface_StoryEncounter
	=========================================================================================*/
public:
	virtual void InitializeStoryEncounter_Implementation(FDataTableRowHandle EncounterData, bool bIsFirstScreen) override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Layout Cos", meta=(BindWidget))
	UWidgetSwitcher* DeckListSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Layout Cos", meta=(BindWidget))
	USizeBox* StoryEncounterBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Layout Cos", meta=(BindWidget))
	UButton* DebugButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Layout Cos", meta=(BindWidget))
	UUW_RewardScreen* WBP_RewardScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Layout Cos", meta=(BindWidget))
	UUW_ScreenFade* WBP_ScreenFade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Layout Cos", meta=(BindWidget))
	UUW_Shop* WBP_Shop;
};
