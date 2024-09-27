#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Interface_Utility.h"
#include "Interfaces/Widget/Interface_StatusWidget.h"
#include "UW_MinionUI.generated.h"

class UHorizontalBox;
class UVerticalBox;
class UUW_StatBar;
class UUW_StatusBar;
class UUW_StatusIcon;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_MinionUI : public UUserWidget, public IInterface_Utility, public IInterface_StatusWidget
{
	GENERATED_BODY()

public:
	UUW_MinionUI(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	/*========================================================================================
	*	IInterface_StatusWidget
	=========================================================================================*/
public:
	virtual UObject* AddStatusIndicator_Implementation(UStatusComponent* Status, bool bShowImmediately) override;

	/*========================================================================================
	*	IInterface_Utility
	=========================================================================================*/
public:
	virtual bool Interact_Implementation(const FGameplayTagContainer& Tags) override;

public:
	UPROPERTY(BlueprintReadWrite, Category="UW Minion UI", meta=(BindWidget))
	TObjectPtr<UHorizontalBox> ActiveStatusBox;
	
	UPROPERTY(BlueprintReadWrite, Category="UW Minion UI", meta=(BindWidget))
	TObjectPtr<UHorizontalBox> PassiveStatusBox;
	
	UPROPERTY(BlueprintReadWrite, Category="UW Minion UI", meta=(BindWidget))
	TObjectPtr<UVerticalBox> StatBarBox;
	
	UPROPERTY(BlueprintReadWrite, Category="UW Minion UI", meta=(BindWidget))
	TObjectPtr<UHorizontalBox> OverlapBarBox;
	
	UPROPERTY(BlueprintReadWrite, Category="UW Minion UI", meta=(BindWidget))
	TObjectPtr<UCanvasPanel> TargetFrame;
	
	UPROPERTY(BlueprintReadWrite, Category="UW Minion UI", meta=(BindWidget))
	TObjectPtr<UUW_StatBar> WBP_StatBar;
	
	UPROPERTY(BlueprintReadWrite, Category="UW Minion UI", meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> ActiveStatusWobble;
	
	UPROPERTY(BlueprintReadWrite, Category="UW Minion UI", meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FrameZoom;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Minion UI|Config|Setup")
	TSubclassOf<UUW_StatusIcon> WBP_StatusIconClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Minion UI|Config|Setup")
	TSubclassOf<UUW_StatBar> WBP_StatBarClass;
};
