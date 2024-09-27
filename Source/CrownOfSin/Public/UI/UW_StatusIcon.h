#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Widget/Interface_StatusIcon.h"
#include "UW_StatusIcon.generated.h"

class UImage;
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

public:
	UFUNCTION(BlueprintCallable, Category="UW Status Icon Event")
	void RefreshAppearance();

	/**
	 * 상태 아이콘의 ToolTip을 상태 정보에 따라 업데이트합니다.
	 * ToolTip 데이터는 상태의 Appearance에서 가져오며, ToolTip이 1개 또는 여러 개일 때 각각의 위젯을 설정합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Status Icon Event")
	void UpdateTooltipFromStatus();


	/*========================================================================================
	*	Blueprint Implementable Event
	=========================================================================================*/
public:
	UFUNCTION(BlueprintCallable, Category="UW Status Icon Event")
	void FadeAway(bool bDestroyAfterFade);

	/**
	 * 상태의 텍스트를 설정하고, 텍스트의 정렬 방식에 따라 적절한 텍스트 블록의 위치와 가시성을 조정합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Status Icon Event")
	void AlignValueTextFromStatus();

	UFUNCTION(BlueprintCallable, Category="UW Status Icon Event")
	void UpdateAppearance();

	/**
	 * 주어진 텍스트 블록의 폰트 크기를 상대 스케일 값에 따라 조정합니다.
	 * 텍스트 블록의 기존 폰트 크기를 상대 스케일에 맞춰 업데이트합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Status Icon Event")
	void UpdateFontSize(UTextBlock* Text, float RelativeScale);

	UFUNCTION(BlueprintCallable, Category="UW Status Icon Event")
	void UpdateSizeBoxSize(USizeBox* SizeBox, float RelativeScale);

protected:
	UFUNCTION()
	void OnClicked_StatusButton();


	/*========================================================================================
	 *	IInterface_StatusIcon
	 =========================================================================================*/
public:
	virtual void InitStatusWidget_Implementation(UStatusComponent* InStatus, FGameplayTag InStatusBarTag, bool bIsShowImmediately) override;

	/**
	 * 상태 위젯을 수정하여 새로운 값과 외형을 업데이트합니다.
	 * 상태 값에 따라 텍스트를 설정하고, 필요 시 애니메이션을 실행합니다.
	 */
	virtual bool ModifyStatusWidget_Implementation(int32 NewValue, const FText& TextOverride, bool bRefreshAppearance, const FStatusAppearance& StatusAppearanceIn, bool bCanBeZero) override;


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadOnly, Category="UW Status Icon|Designer", meta=(BindWidget))
	TObjectPtr<UImage> StatusImage;

	UPROPERTY(BlueprintReadOnly, Category="UW Status Icon|Designer", meta=(BindWidget))
	TObjectPtr<UButton> StatusButton;

	UPROPERTY(BlueprintReadOnly, Category="UW Status Icon|Designer", meta=(BindWidget))
	TObjectPtr<UTextBlock> BottomRightNumber;

	UPROPERTY(BlueprintReadOnly, Category="UW Status Icon|Designer", meta=(BindWidget))
	TObjectPtr<UTextBlock> CenterNumber;

	UPROPERTY(BlueprintReadOnly, Category="UW Status Icon|Designer", meta=(BindWidget))
	TObjectPtr<UTextBlock> BottomLeftNumber;

	UPROPERTY(BlueprintReadOnly, Category="UW Status Icon|Designer", meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> AlertModify;

	UPROPERTY(BlueprintReadOnly, Category="UW Status Icon|Designer", meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FlyUp;

	UPROPERTY(BlueprintReadOnly, Category="UW Status Icon|Designer", meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> NoticeWobble;

	UPROPERTY(BlueprintReadOnly, Category="UW Status Icon|Designer", meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeOut;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Status Icon|Confing|Setup")
	TSubclassOf<UUW_ToolTip> WBP_ToolTipClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UW Status Icon|Confing|Setup")
	TSubclassOf<UUW_ToolTipList> WBP_ToolTipListClass;

public:
	UPROPERTY(BlueprintReadWrite, Category="UW Status Icon|Component")
	UStatusComponent* StatusComponent;

	UPROPERTY(BlueprintReadWrite, Category="UW Status Icon")
	FGameplayTag StatusBarTag;

	UPROPERTY(BlueprintReadWrite, Category="UW Status Icon")
	FStatusAppearance StatusAppearance;
};
