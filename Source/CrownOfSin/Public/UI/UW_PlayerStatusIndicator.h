#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Widget/Interface_StatusIcon.h"
#include "UW_PlayerStatusIndicator.generated.h"

class UUW_ToolTip;
class UUW_ToolTipList;
class UTextBlock;
class UStatusComponent;


/**
 * UUW_PlayerStatusIndicator 클래스는 플레이어의 상태(Status)를 UI에 표시하는 위젯입니다.
 * 이 위젯은 플레이어의 다양한 상태값(예: 마나, 체력 등)을 숫자로 표시하며, 상태값에 따라 툴팁이나 애니메이션을 적용합니다.
 * 상태값이 업데이트되면 해당 값을 화면에 표시하고, 상태와 관련된 툴팁을 업데이트합니다.
 * 
 * 주요 기능:
 * - 상태값을 실시간으로 업데이트하고 화면에 표시
 * - 상태와 관련된 툴팁을 동적으로 변경
 * - 상태 변화 시 애니메이션을 재생하여 시각적 피드백 제공
 */
UCLASS()
class CROWNOFSIN_API UUW_PlayerStatusIndicator : public UUserWidget, public IInterface_StatusIcon
{
	GENERATED_BODY()

public:
	UUW_PlayerStatusIndicator(const FObjectInitializer& ObjectInitializer);

public:
	/**
	 * 상태값을 업데이트하는 함수입니다.
	 * 새로운 상태값을 화면에 표시하고 애니메이션을 재생합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Player Status Indicator")
	void UpdateValue(int32 NewValue);

	/**
	 * 상태 컴포넌트로부터 툴팁 정보를 가져와 툴팁을 업데이트하는 함수입니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Player Status Indicator")
	void UpdateTooltipFromStatus(UStatusComponent* InStatus);

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
	UPROPERTY(BlueprintReadWrite, Category="UW Player Status Indicator|Designer", meta=(BindWidget))
	UTextBlock* StatText;

	UPROPERTY(BlueprintReadWrite, Category="UW Player Status Indicator|Designer", meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* IncreaseAnim;

public:
	/** 
 	 * 툴팁 클래스를 나타내는 변수
 	 * 상태에 따라 툴팁 정보를 제공하는 데 사용됩니다.
 	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Player Status Indicator|Config|Setup")
	TSubclassOf<UUW_ToolTip> WBP_ToolTipClass;

	/** 
	 * 여러 툴팁을 표시할 수 있는 툴팁 리스트 클래스를 나타내는 변수
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Player Status Indicator|Config|Setup")
	TSubclassOf<UUW_ToolTipList> WBP_ToolTipListClass;

	/** 
	 * 표시할 상태 컴포넌트의 클래스
	 * 이 위젯이 표시할 상태의 유형을 정의합니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Player Status Indicator", meta=(ExposeOnSpawn="true"))
	TSubclassOf<UStatusComponent> StatusClass;

	/** 
	 * 현재 표시 중인 상태 컴포넌트
	 * 이 위젯에 연결된 상태 데이터입니다.
	 */
	UPROPERTY(BlueprintReadWrite, Category="UW Player Status Indicator")
	UStatusComponent* StatComponent;
	
};
