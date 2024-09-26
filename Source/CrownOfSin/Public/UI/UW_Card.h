#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Core/CosEnumStruct.h"
#include "Interfaces/Interface_EventHolder.h"
#include "Interfaces/Widget/Interface_CardWidget.h"
#include "UW_Card.generated.h"


class USizeBox;
class UImage;
class UUW_ToolTipList;
class UScaleBox;
class UUW_CardVisual;
class UUW_TargetingBezier;
class UUW_CardHand;
class ACardBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCardWidgetEvent, FGameplayTag, InEvent, UUW_Card*, InCardWidget);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRemoved, ACardBase*, InRemovedCard, UUserWidget*, InRemovedWidget);

/**
 * 
 */
UCLASS()
class CROWNOFSIN_API UUW_Card : public UUserWidget, public IInterface_EventHolder, public IInterface_CardWidget
{
	GENERATED_BODY()

public:
	UUW_Card(const FObjectInitializer& ObjectInitializer);

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	/**
	 * 카드의 상태에 따라 매 프레임 실행되는 동작을 처리하는 함수.
	 * 상태에 따라 카드를 이동시키거나 타겟 모드, 마우스 추적 등을 수행함.
	 */
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/**
	 * 마우스가 카드 위에 올라왔을 때 호출되는 함수.
	 * 카드의 상태를 'MouseOver'로 변경하고, 카드의 위치와 Z-Order를 조정하여 시각적 효과를 처리함.
	 */
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	/**
	 * 마우스가 카드에서 벗어났을 때 호출되는 함수.
	 * 카드의 상태를 'InHand'로 복구하고, 크기 및 Z-Order를 초기 상태로 되돌리며 UI 가시성을 처리함.
	 */
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	/**
	 * 카드 위젯에서 마우스 버튼이 눌렸을 때 호출되는 함수.
	 * 카드 상태에 따라 타겟팅, 플레이, 마우스 추적 등 적절한 동작을 처리하고, 관련 이벤트를 브로드캐스트함.
	 */
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	/**
 	 * 마우스가 카드 위에 올라왔을 때 애니메이션을 실행하고 상단 가시성을 변경하는 함수.
 	 * 첫 마우스 오버 이벤트 발생 시에만 애니메이션을 실행하며, 이후 가시성을 처리.
 	 */
	void MoveOnMouseOver();

	/**
	 * 카드 위젯에서 마우스 오버 효과를 초기화하는 함수.
	 * 툴팁을 숨기고, 마우스 오버 애니메이션을 반대로 재생하며, 마우스 오버 상태 플래그를 리셋함.
	 */
	void ResetHoverZoom();

public:
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void AnimateCardEvent(FGameplayTag ActionTag);

	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void AnimateCardTransfer(FGameplayTag PileTag);

	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void DestroyAfterDelay(float InDelay);

	/**
	 * 카드 릴리스 후 처리할 결과를 결정하는 함수.
	 * 카드를 성공적으로 사용한 경우 활성화 상태로 전환하고, 그렇지 않으면 카드를 손으로 되돌림.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void DetermineResultOfCardRelease();

	/**
	 * 카드를 손으로 되돌리고, 카드의 상태와 시각적 요소를 초기화하는 함수.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void ReturnCardToHand();

	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void SetupBezierFollow(const FTransferAnim& NewTransferAnimData);

	/**
	 * 카드가 베지어 곡선을 따라 이동하며 애니메이션을 처리하는 함수.
	 * 베지어 곡선의 진행도를 기반으로 위치, 크기, 각도를 업데이트하고, 곡선이 끝나면 카드가 제거됨.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void FollowBezier();

	/**
	 * 카드의 현재 변환 상태를 목표 변환 상태로 부드럽게 이동시키는 함수.
	 * 위치, 크기, 각도를 일정한 속도로 보간하며, 비교 임계값을 이용해 변환 완료 여부를 확인함.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void MoveToSuggestedTransform(bool bContinueWhenDone, const FWidgetTransform& InSuggestedTransform, float Speed);

	/**
	 * 마우스를 따라 카드를 이동시키고, 카드가 활성화될 조건을 확인하는 함수.
	 * 마우스의 Y 좌표와 카드 영역을 비교하여 카드의 활성화 또는 비활성화를 처리함.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void FollowMouseAndCheckActivation();

	/**
	 * 카드를 타겟팅 모드로 전환하고, 마우스 커서 아래의 타겟을 감지하여 상호작용하는 함수.
	 * 마우스 커서가 오브젝트를 가리킬 때 해당 오브젝트가 타겟인 경우 추가 처리를 수행함.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void RunTargetingMode();

	/**
	 * 마킹된 타겟을 해제하고 카드의 발광 상태를 초기화하는 함수.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void UnmarkMarkedActors();

	/**
	 * 카드가 활성화될 수 있는 조건을 확인하고, 유효한 경우 카드를 활성화하는 함수.
	 * 조건이 충족되지 않으면 실패 메시지를 출력하고 카드를 손으로 되돌림.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void ActivateCardIfValid();

	/**
	 * 카드를 비활성화하고 마킹된 타겟을 해제하는 함수.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void DeactivateCard();

	/**
	 * 카드의 발광 상태를 플레이 가능 여부에 따라 업데이트하는 함수.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void ResetGlow();

	/**
	 * 마우스 릴리스 이벤트를 바인딩하는 함수.
	 * 마우스가 클릭된 후 일정 거리 이상 이동하면 마우스 릴리스 이벤트를 글로벌 디스패처에 바인딩함.
	 */
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void BindToMouseRelease();

protected:
	/**
	 * 마우스가 클릭 위치에서 일정 거리 이상 이동했을 때 릴리스 이벤트를 바인딩하는 함수.
	 * 일정 시간 동안 마우스 위치를 계속 추적하여 조건을 만족하면 릴리스 이벤트를 바인딩함.
	 */
	UFUNCTION()
	void DelayBindToMouseRelease();

	/**
	 * 카드가 업데이트되었을 때 호출되는 함수.
	 * 카드의 UI를 갱신하고, 애니메이션을 실행하여 카드가 업데이트된 것을 시각적으로 반영함.
	 */
	UFUNCTION()
	void UpdateOnCardUpdate();

	/**
	 * 타겟팅 모드에서 선택된 타겟을 처리하는 함수.
	 * 선택된 타겟이 유효한 경우 마킹하고 카드의 상태를 업데이트함.
	 */
	UFUNCTION()
	void HandleRunTargetingMode(AActor* TargetingActor);


	/*========================================================================================
	*	IInterface_CardWidget
	=========================================================================================*/
public:
	virtual void AnimateAutoPlay_Implementation() override;

	virtual void ToggleCardGlow_Implementation(bool bEnable, FLinearColor Color) override;

	virtual void UpdateCardWidget_Implementation(ACardBase* InCardActor) override;

	virtual void RequestDesiredTransformUpdate_Implementation(FWidgetTransform DesiredTransform, bool bEnableMovement) override;

	virtual void SuggestCardZOrder_Implementation(int32 ZOrder) override;

	virtual void ForceCardZOrder_Implementation(int32 ZOrder) override;

	/*========================================================================================
	*	IInterface_EventHolder
	=========================================================================================*/
public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category="UW Card|Desinger", meta=(BindWidget))
	TObjectPtr<USizeBox> CardVisualBox;

	UPROPERTY(BlueprintReadWrite, Category="UW Card|Desinger", meta=(BindWidget))
	TObjectPtr<UImage> HoverZoneBottom;

	UPROPERTY(BlueprintReadWrite, Category="UW Card|Desinger", meta=(BindWidget))
	TObjectPtr<UImage> HoverZoneTop;

	UPROPERTY(BlueprintReadWrite, Category="UW Card|Desinger", meta=(BindWidget))
	TObjectPtr<UUW_ToolTipList> WBP_TooltipList;

	UPROPERTY(BlueprintReadWrite, Category="UW Card|Desinger", meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> HoverMove;

	UPROPERTY(BlueprintReadWrite, Category="UW Card|Desinger", meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> Activate;

	UPROPERTY(BlueprintReadWrite, Category="UW Card|Desinger", meta=(BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> HideDown;

public:
	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	bool bHovering = false;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	bool bVisuallyInHand = false;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	bool bActive = false;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	int32 SuggestedZOrder = 0;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	int32 HoverZOrder = 100;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	float MouseFollowSpeed = 20.0f;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	float DestroyDelay = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	float HandScale = 0.6f;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	float HoverScale = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	float HoverOffsetJumpPropY = 0.85f;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	float UE5HoverOffset = -60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card", meta=(ExposeOnSpawn="true"))
	float HoverOffsetY = -110.0f;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	float CardMoveSpeed = 9.0f;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	float AutoPlayMoveSpeed = 6.0f;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	float BezierAlpha = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	float TargetingModeYOffset = 900.0f;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	ECardState CardState = ECardState::InHand;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	FWidgetTransform SuggestedTransform;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	FVector2D MousePositionOnClick;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	FLinearColor GlowActive;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	FLinearColor GlowValid;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	FTransferAnim TransferAnimData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card", meta=(ExposeOnSpawn="true"))
	TSubclassOf<UUW_CardVisual> CardVisualClass;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	TObjectPtr<UUW_CardVisual> CardVisual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card", meta=(ExposeOnSpawn="true"))
	TObjectPtr<ACardBase> CardActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card", meta=(ExposeOnSpawn="true"))
	TObjectPtr<UUW_CardHand> HandReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card", meta=(ExposeOnSpawn="true"))
	TObjectPtr<UUW_TargetingBezier> TargetArrowReference;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	TArray<TObjectPtr<AActor>> MarkedActors;

private:
	bool bMouseOverFlag = true;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="UW Card|Delegate")
	FOnCardWidgetEvent OnCardWidgetEvent;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="UW Card|Delegate")
	FOnRemoved OnRemoved;

private:
	FTimerHandle MouseLeaveTimerHandle;
	FTimerHandle HoverEffectTimerHandle;
	FTimerHandle MouseReleaseBindTimerHandle;
};
