#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Widget/Interface_StatusIcon.h"
#include "UW_StatusIcon.generated.h"

class UUW_ToolTipList;
class UUW_ToolTip;
class USizeBox;
class UTextBlock;
class UStatusComponent;
class UButton;
/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_StatusIcon : public UUserWidget, public IInterface_StatusIcon
{
	GENERATED_BODY()

public:
	UUW_StatusIcon(const FObjectInitializer& Initializer);

	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnClicked_StatusButton();

public:
	UFUNCTION(BlueprintCallable, Category="UW Status Icon Event")
	void RefreshAppearance();

	UFUNCTION(BlueprintCallable, Category="UW Status Icon Event")
	void UpdateTooltipFromStatus();


	/*========================================================================================
	*	Blueprint Implementable Event
	=========================================================================================*/
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="UW Status Icon Event")
	void FadeAway(bool bDestroyAfterFade);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="UW Status Icon Event")
	void AlignValueTextFromStatus();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="UW Status Icon Event")
	void UpdateAppearance();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="UW Status Icon Event")
	void UpdateFontSize(UTextBlock* Text, float RelativeScale);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="UW Status Icon Event")
	void UpdateSizeBoxSize(USizeBox* SizeBox, float RelativeScale);


	/*========================================================================================
	 *	IInterface_StatusIcon
	 =========================================================================================*/
public:
	virtual void InitStatusWidget_Implementation(UStatusComponent* InStatus, FGameplayTag InStatusBarTag, bool bIsShowImmediately) override;

	virtual bool ModifyStatusWidget_Implementation(int32 NewValue, const FText& TextOverride, bool bRefreshAppearance, const FStatusAppearance& StatusAppearanceIn, bool bCanBeZero) override;


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Status Icon|Confing|Setup")
	TSubclassOf<UUW_ToolTip> WBP_ToolTipClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Status Icon|Confing|Setup")
	TSubclassOf<UUW_ToolTipList> WBP_ToolTipListClass;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Status Icon|Designer", meta=(BindWidget))
	UButton* StatusButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Status Icon|Designer", meta=(BindWidget))
	UTextBlock* BottomRightNumber;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Status Icon|Designer", meta=(BindWidget))
	UTextBlock* CenterNumber;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Status Icon|Designer", meta=(BindWidget))
	UTextBlock* BottomLeftNumber;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Status Icon|Designer", meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* AlertModify;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Status Icon|Designer", meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* FlyUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Status Icon|Designer", meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* NoticeWobble;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Status Icon|Designer", meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* FadeOut;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Status Icon|Component")
	UStatusComponent* StatusComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Status Icon")
	FGameplayTag StatusBarTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Status Icon")
	FStatusAppearance StatusAppearance;


};
