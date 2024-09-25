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

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	void MoveOnMouseOver();

	void ResetHoverZoom();

	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void AnimateCardEvent(FGameplayTag ActionTag);

	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void AnimateCardTransfer(FGameplayTag PileTag);

	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void DestroyAfterDelay(float InDelay);

	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void DetermineResultOfCardRelease();

	
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void ReturnCardToHand();

	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void SetupBezierFollow(const FTransferAnim& NewTransferAnimData);

	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void FollowBezier();

	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void MoveToSuggestedTransform(bool bContinueWhenDone,const FWidgetTransform& InSuggestedTransform,float Speed);
	
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void FollowMouseAndCheckActivation();
	
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void RunTargetingMode();
	
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void UnmarkMarkedActors();
	
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void HandleRunTargetingMode(AActor* TargetingActor);
	
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void ActivateCardIfValid();
	
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void DeactivateCard();
	
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void ResetGlow();
	
	UFUNCTION(BlueprintCallable, Category="UW Card Event")
	void BindToMouseRelease();

protected:
	UFUNCTION()
	void UpdateOnCardUpdate();

	UFUNCTION()
	void DelayBindToMouseRelease();


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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card|Desinger", meta=(BindWidget))
	USizeBox* CardVisualBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card|Desinger", meta=(BindWidget))
	UImage* HoverZoneBottom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card|Desinger", meta=(BindWidget))
	UImage* HoverZoneTop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card|Desinger", meta=(BindWidget))
	UUW_ToolTipList* WBP_TooltipList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card|Desinger", meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* HoverMove;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card|Desinger", meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* Activate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card|Desinger", meta=(BindWidgetAnim), Transient)
	UWidgetAnimation* HideDown;

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
	float UE5HoverOffset = 0.85f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card", meta=(ExposeOnSpawn="true"))
	float HoverOffsetY = -110.0f;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	float CardMoveSpeed = 9.0f;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	float AutoPlayMoveSpeed = 6.0f;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	float BezierAlpha;

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
	UUW_CardVisual* CardVisual;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card", meta=(ExposeOnSpawn="true"))
	ACardBase* CardActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card", meta=(ExposeOnSpawn="true"))
	UUW_CardHand* HandReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UW Card", meta=(ExposeOnSpawn="true"))
	UUW_TargetingBezier* TargetArrowReference;

	UPROPERTY(BlueprintReadWrite, Category="UW Card")
	TArray<AActor*> MarkedActors;

private:
	bool bMouseOver = true;

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="UW Card|Delegate")
	FOnCardWidgetEvent OnCardWidgetEvent;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="UW Card|Delegate")
	FOnRemoved OnRemoved;
};