#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Widget/Interface_StatusWidget.h"
#include "UW_RewardScreen.generated.h"

class UTextBlock;
class UUW_StatusBar;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReturnSelection, bool, bIsSkipped, UStatusComponent*, InArtifact);

/**
 * UUW_RewardScreen 클래스는 보상 화면을 관리하는 UI 위젯입니다.
 * 게임 내에서 플레이어가 보상을 선택하거나 스킵할 수 있는 기능을 제공합니다.
 * 보상은 주로 상태(Status) 형태로 제공되며, 상태 목록을 표시하고 상호작용할 수 있습니다.
 * 플레이어가 '모두 수령' 버튼을 눌러 보상을 한 번에 수령하거나, '건너뛰기' 버튼을 눌러 보상을 스킵할 수 있습니다.
 * 
 * 주요 기능:
 * - 보상 화면을 표시하고 애니메이션 재생
 * - 플레이어가 보상을 수령하거나 스킵할 수 있는 버튼 제공
 * - 보상을 모두 수령하거나 남은 보상이 없을 경우 자동으로 화면을 닫음
 */
UCLASS()
class CROWNOFSIN_API UUW_RewardScreen : public UUserWidget, public IInterface_StatusWidget
{
	GENERATED_BODY()

public:
	UUW_RewardScreen(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	/**
	 * 보상 화면을 나타내는 함수입니다.
	 * 보상 화면을 표시하고 FadeIn 애니메이션을 재생합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Reward Screen Event")
	void Appear();

protected:
	/**
	 * '모두 수령' 버튼 클릭 시 호출되는 함수입니다.
	 * 플레이어가 화면에 있는 모든 보상을 수령합니다.
	 */
	UFUNCTION()
	void OnClicked_TakeAllButton();

	/**
	 * '건너뛰기' 버튼 클릭 시 호출되는 함수입니다.
	 * 보상을 건너뛰고 보상 화면을 닫습니다.
	 */
	UFUNCTION()
	void OnClicked_SkipButton();

	/*========================================================================================
	*	IInterface_StatusWidget
	=========================================================================================*/
public:
	/**
	 * 상태(Status) 인디케이터를 추가하는 함수입니다.
	 * 보상으로 상태 컴포넌트를 화면에 표시합니다.
	 */
	virtual UObject* AddStatusIndicator_Implementation(UStatusComponent* Status, bool bShowImmediately) override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category="UW Reward Screen|Designer", meta=(BindWidget))
	UButton* TakeAllButton;

	UPROPERTY(BlueprintReadWrite, Category="UW Reward Screen|Designer", meta=(BindWidget))
	UButton* SkipButton;

	UPROPERTY(BlueprintReadWrite, Category="UW Reward Screen|Designer", meta=(BindWidget))
	UTextBlock* ButtonText;

	UPROPERTY(BlueprintReadWrite, Category="UW Reward Screen|Designer", meta=(BindWidget))
	UUW_StatusBar* WBP_ArtifactBar;

	UPROPERTY(BlueprintReadWrite, Category="UW Reward Screen|Designer", meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* FadeIn;

public:
	/** 
	 * 보상이 없을 때 화면을 자동으로 닫을지 여부를 결정하는 플래그
	 * 보상이 모두 소진되면 화면을 자동으로 닫습니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Reward Screen")
	bool bCloseWhenEmpty = true;

public:
	/** 
	 * 보상 선택 이벤트가 완료되었을 때 호출되는 델리게이트
	 * 보상을 수령하거나 스킵했을 때 이 델리게이트가 실행됩니다.
	 */
	UPROPERTY(Blueprintable, BlueprintCallable, Category="UW Reward Screen|Delegate")
	FOnReturnSelection OnReturnSelection;
};
