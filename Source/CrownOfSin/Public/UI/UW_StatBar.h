#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Interface_EventHolder.h"
#include "Interfaces/Widget/Interface_StatusIcon.h"
#include "UW_StatBar.generated.h"


class UTextBlock;
class UProgressBar;
class UStatusComponent;

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_StatBar : public UUserWidget, public IInterface_StatusIcon, public IInterface_EventHolder
{
	GENERATED_BODY()

public:
	UUW_StatBar(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

public:
	/**
	 * 애니메이션 바의 감소를 처리하고 필요한 경우 재귀적으로 호출하여
	 * 부드러운 상태 변화를 시뮬레이션하는 함수입니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW StatBar")
	void AnimateDecrease();

	/**
	 * 상태 바 텍스트를 현재 상태 값과 최대 상태 값으로 업데이트합니다.
	 * 텍스트는 "현재 값 / 최대 값" 형식으로 표시됩니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW StatBar")
	void UpdateNumberText(int32 NewValue);

	/**
 	 * 상태 바의 현재 상태 값을 퍼센트(0.0 ~ 1.0)로 반환합니다.
 	 * MaxStatusValue가 0이 아닐 때만 계산하며, 퍼센트 값은 0과 1 사이로 클램핑됩니다.
 	 */
	UFUNCTION(BlueprintPure, Category="UW StatBar")
	float GetBarPercent();

protected:
	/**
	 * 애니메이션 바의 감소 상태를 업데이트하고, 일정한 속도로 감소시킵니다.
	 */
	UFUNCTION()
	void SmoothAnimateBarDecrease();

	/*========================================================================================
	*	IInterface_EventHolder
	=========================================================================================*/
public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

	/*========================================================================================
	*	IInterface_StatusIcon
	=========================================================================================*/
public:
	/**
	 * 상태 위젯을 초기화하고, 상태 값을 기반으로 StatBar와 AnimateBar의 초기 퍼센트를 설정합니다.
	 * 또한, 상태가 변화할 수 있는지 여부에 따라 MaxStatusValue를 설정합니다.
	 */
	virtual void InitStatusWidget_Implementation(UStatusComponent* InStatus, FGameplayTag InStatusBarTag, bool bIsShowImmediately) override;

	/**
	 * 새로운 상태 값을 적용하고, 상태 바를 업데이트합니다.
	 * 상태 값에 따라 텍스트 및 퍼센트가 갱신되며, 애니메이션이 진행됩니다.
	 */
	virtual bool ModifyStatusWidget_Implementation(int32 NewValue, const FText& TextOverride, bool bRefreshAppearance, const FStatusAppearance& StatusAppearanceIn, bool bCanBeZero) override;


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category="UW StatBar", meta=(BindWidget))
	TObjectPtr<UTextBlock> NumberText;

	UPROPERTY(BlueprintReadWrite, Category="UW StatBar", meta=(BindWidget))
	TObjectPtr<UProgressBar> StatBar;

	UPROPERTY(BlueprintReadWrite, Category="UW StatBar", meta=(BindWidget))
	TObjectPtr<UProgressBar> AnimateBar;

public:
	UPROPERTY(BlueprintReadWrite, Category="UW StatBar")
	int32 MaxStatusValue = 0;

	UPROPERTY(BlueprintReadWrite, Category="UW StatBar")
	TObjectPtr<UStatusComponent> StatusComponent;
};
