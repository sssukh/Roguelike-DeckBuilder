
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Interface_StoryEncounter.h"
#include "Interfaces/Widget/Interface_StatusWidget.h"
#include "UW_Layout_Cos.generated.h"

class UUW_StatusBar;
class UUW_CardRewardScreen;
class UUW_CardSelectorList;
class UUW_ArtifactRewardScreen;
class UUW_CardHand;
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
class CROWNOFSIN_API UUW_Layout_Cos : public UUserWidget, public IInterface_StoryEncounter, public IInterface_StatusWidget
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
	
	/*ToDo:구현해야합니다.*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="UW Layout")
	void DisplayScreenLogMessage(const FText& Message, const FColor& Color);

	/*ToDo:구현해야합니다.*/
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="UW Layout")
	void ToggleInGameMenu();

	/*ToDo:구현해야합니다.*/
	void UpdatePileWidgetAmount(FGameplayTag PileTag, int32 NewAmount);


	/*ToDo 구현해야합니다*/
	UFUNCTION(BlueprintPure, Category="UW LayOut")
	bool GetEndTurnButtonIsEnabled();
	
	/*ToDo 구현해야합니다*/
	UFUNCTION(BlueprintPure, Category="UW LayOut")
	ESlateVisibility GetCurrencyBoxVisibility();

	/*========================================================================================
	*	Iinterface_StoryEncounter
	=========================================================================================*/
public:
	virtual void InitializeStoryEncounter_Implementation(FDataTableRowHandle EncounterData, bool bIsFirstScreen) override;

	/*========================================================================================
	*	Iinterface_StoryEncounter
	=========================================================================================*/
public:
	virtual UObject* AddStatusIndicator_Implementation(UStatusComponent* Status, bool bShowImmediately) override;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Layout Cos", meta=(BindWidget))
	TObjectPtr<UUW_CardHand> WBP_Hand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Layout Cos", meta=(BindWidget))
	TObjectPtr<UUW_ArtifactRewardScreen> WBP_ArtifactReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Layout Cos", meta=(BindWidget))
	TObjectPtr<UUW_CardSelectorList> WBP_CardSelectorList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Layout Cos", meta=(BindWidget))
	TObjectPtr<UUW_CardRewardScreen> WBP_CardRewardScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Layout Cos", meta=(BindWidget))
	TObjectPtr<UUW_StatusBar> WBP_ArtifactBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Layout Cos", meta=(BindWidget))
	TObjectPtr<UUW_StatusBar> WBP_CurrencyBar;
};
