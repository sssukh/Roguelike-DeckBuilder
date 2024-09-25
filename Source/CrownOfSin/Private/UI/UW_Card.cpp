#include "UI/UW_Card.h"

#include "Animation/WidgetAnimation.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "CardSystem/CardBase.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/ScaleBox.h"
#include "Components/SizeBox.h"
#include "Core/MinionBase.h"
#include "Core/TurnManager.h"
#include "Interfaces/Interface_CardPuppet.h"
#include "Interfaces/Interface_CardTarget.h"
#include "Interfaces/Widget/Interface_TargetArrow.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Libraries/FunctionLibrary_Utility.h"
#include "UI/UW_CardHand.h"
#include "UI/UW_CardVisual.h"
#include "UI/UW_ToolTipList.h"
#include "UI/UW_TargetingBezier.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"

UUW_Card::UUW_Card(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GlowActive = FLinearColor(0, 1, 0, 1);
	GlowValid = FLinearColor(1, 1, 1, 1);
}

void UUW_Card::NativePreConstruct()
{
	Super::NativePreConstruct();

	COS_IF_CHECK(CardVisualClass, TEXT("UW Card에서 CardVisualClass를 설정해주세요."));

	if (IsValid(CardActor))
	{
		CardVisualClass = CardActor->GetCardVisualWidget(ECardDataType::Hand);
	}

	CardVisual = CreateWidget<UUW_CardVisual>(GetWorld(), CardVisualClass);

	CardVisualBox->AddChild(CardVisual);
	CardVisual->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UUW_Card::NativeConstruct()
{
	Super::NativeConstruct();

	if (!IsValid(CardActor)) return;

	IInterface_CardWidget::Execute_UpdateCardWidget(CardVisual, CardActor);

	ResetGlow();

	CardActor->OnThisCardModified.AddDynamic(this, &UUW_Card::UpdateOnCardUpdate);

	for (const FDataTableRowHandle& CardTooltip : CardActor->GetCardTooltips(ECardDataType::Hand))
	{
		WBP_TooltipList->AddToolTipFromData(CardTooltip, 0);
	}
}

void UUW_Card::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CardState == ECardState::InHand || CardState == ECardState::Activating)
	{
		MoveToSuggestedTransform(false, SuggestedTransform, CardMoveSpeed);
	}
	else if (CardState == ECardState::FollowMouse)
	{
		FollowMouseAndCheckActivation();
	}
	else if (CardState == ECardState::Targeting)
	{
		RunTargetingMode();
	}
	else if (CardState == ECardState::FollowBelzier)
	{
		FollowBezier();
	}
}

void UUW_Card::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!IsValid(HandReference->HeldCard))
	{
		if (CardState == ECardState::InHand && !bActive && CardActor->IsInHand() && bVisuallyInHand)
		{
			CardState = ECardState::MouseOver;

			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot);
			CanvasSlot->SetZOrder(HoverZOrder);
			FVector2D PileHandViewPosition = HandReference->GetAnchorWidgetPosition(CosGameTags::Pile_Hand);

			const FWidgetTransform& CurrentRenderTransform = GetRenderTransform();
			FWidgetTransform NewRenderTransform;

			float NewTranslationY = PileHandViewPosition.Y + ((HoverOffsetY * HoverOffsetJumpPropY) + UE5HoverOffset);
			NewRenderTransform.Translation = FVector2D(CurrentRenderTransform.Translation.X, NewTranslationY);
			NewRenderTransform.Scale = FVector2D(HoverScale);
			NewRenderTransform.Shear = FVector2D::ZeroVector;
			NewRenderTransform.Angle = 0.0f;
			SetRenderTransform(NewRenderTransform);

			MoveOnMouseOver();
		}
	}
}

void UUW_Card::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (CardState == ECardState::MouseOver)
	{
		if (!IsValid(HandReference->HeldCard))
		{
			CardState = ECardState::InHand;
			SetRenderScale(FVector2D(HandScale));
			UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Slot);
			CanvasPanelSlot->SetZOrder(SuggestedZOrder);
			ResetHoverZoom();
			FTimerHandle MouseLeaverTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(MouseLeaverTimerHandle, [&]()
			{
				HoverZoneTop->SetVisibility(HoverZoneBottom->GetVisibility());
			}, 0.3f, false);
		}
	}
}

FReply UUW_Card::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (OnCardWidgetEvent.IsBound())
		OnCardWidgetEvent.Broadcast(CosGameTags::Event_Card_Grab, this);

	if (CardState == ECardState::MouseOver && !HandReference->bSelectionMode && HandReference->bEnabled)
	{
		ATurnManager* TurnManager = Cast<ATurnManager>(UGameplayStatics::GetActorOfClass(this, ATurnManager::StaticClass()));
		if (TurnManager->bCombatOver)
		{
			if (OnCardWidgetEvent.IsBound())
				OnCardWidgetEvent.Broadcast(CosGameTags::Event_Card_Drop, this);
			return FReply::Handled();
		}

		BindToMouseRelease();
		HandReference->HeldCard = this;
		bHovering = false;

		UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Slot);
		CanvasPanelSlot->SetZOrder(HoverZOrder);
		WBP_TooltipList->SetVisibility(ESlateVisibility::Collapsed);

		if (CardActor->GetCardTargeted(ECardDataType::Hand))
		{
			FString FailMessage;
			bool bPlayable = CardActor->CheckIfPlayable(FailMessage);

			if (CardActor->IsInHand() && bPlayable)
			{
				CardState = ECardState::Targeting;
				FVector2D SourcePoint = GetRenderTransform().Translation + FVector2D(0, -170.0f);
				IInterface_TargetArrow::Execute_UpdateTargetArrow(TargetArrowReference, true, FLinearColor(1, 1, 1, 1), true, SourcePoint, false);
			}
			else
			{
				UFunctionLibrary_Utility::SendScreenLogMessage(this, FText::FromString(FailMessage), FColor(1, 0, 0));
				if (OnCardWidgetEvent.IsBound())
					OnCardWidgetEvent.Broadcast(CosGameTags::Event_Card_Drop, this);

				ReturnCardToHand();
			}
			return FReply::Handled();
		}
		else
		{
			CardState = ECardState::FollowMouse;
			return FReply::Handled();
		}
	}
	else
	{
		if (OnCardWidgetEvent.IsBound())
			OnCardWidgetEvent.Broadcast(CosGameTags::Event_Card_Drop, this);

		return FReply::Handled();
	}
}

void UUW_Card::MoveOnMouseOver()
{
	if (bMouseOver)
	{
		bMouseOver = false;

		PlayAnimation(HoverMove);

		FTimerHandle MouseOverTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(MouseOverTimerHandle, [&]()
		{
			HoverZoneTop->SetVisibility(ESlateVisibility::Hidden);
		}, 0.1f, false);
	}
}

void UUW_Card::ResetHoverZoom()
{
	WBP_TooltipList->SetVisibility(ESlateVisibility::Collapsed);
	ReverseAnimation(HoverMove);
	bMouseOver = true;
}

void UUW_Card::AnimateCardEvent(FGameplayTag ActionTag)
{
	SuggestedTransform = GetRenderTransform();

	HandReference->UpdateCardTransforms();

	if (ActionTag == CosGameTags::Event_Action_FlickCard)
	{
		const FWidgetTransform& CurrentRenderTransform = GetRenderTransform();
		SuggestedTransform.Translation = FVector2D(CurrentRenderTransform.Translation.X, CurrentRenderTransform.Translation.Y - 600.0f);
		SuggestedTransform.Angle = 160.0f;
		SuggestedTransform.Scale = CurrentRenderTransform.Scale;
		SuggestedTransform.Shear = CurrentRenderTransform.Shear;
		return;
	}
	if (ActionTag == CosGameTags::Event_Action_UseCard)
	{
		PlayAnimation(Activate);

		FVector2D ViewportPosition = HandReference->GetAnchorWidgetPosition(CosGameTags::Event_Card_PrePlay);
		SuggestedTransform.Translation = ViewportPosition + FVector2D(100.0f, -100.0f);
		SuggestedTransform.Scale = FVector2D(0.5f, 0.5f);
		SuggestedTransform.Shear = FVector2D::Zero();
		SuggestedTransform.Angle = 0.0f;
	}
	if (ActionTag == CosGameTags::Event_Action_AutoPlay)
	{
		IInterface_CardWidget::Execute_AnimateAutoPlay(this);
	}
	if (ActionTag == CosGameTags::Event_CardSelectionMode)
	{
		IInterface_CardWidget::Execute_AnimateCardAppear(CardVisual, EUMGSequencePlayMode::Reverse);
	}
}

void UUW_Card::AnimateCardTransfer(FGameplayTag PileTag)
{
	HandReference->UpdateCardTransforms();

	if (PileTag == CosGameTags::Pile_Exhaust || PileTag == CosGameTags::Pile_Void || PileTag == CosGameTags::Pile_Destroy)
	{
		IInterface_CardWidget::Execute_AnimateCardExhaust(CardVisual, EUMGSequencePlayMode::Forward);
		DestroyAfterDelay(DestroyDelay);
	}
	else
	{
		FVector2D ViewportPosition = HandReference->GetAnchorWidgetPosition(PileTag);
		SuggestedTransform.Translation = ViewportPosition;
		SuggestedTransform.Scale = FVector2D::Zero();
		SuggestedTransform.Shear = FVector2D::Zero();
		SuggestedTransform.Angle = 0.0f;
		DestroyAfterDelay(DestroyDelay);
	}
}

void UUW_Card::DestroyAfterDelay(float InDelay)
{
	FTimerHandle DelayTimer;
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, [&]()
	{
		if (OnRemoved.IsBound())
			OnRemoved.Broadcast(CardActor, this);
		RemoveFromParent();
	}, InDelay, false);
}

void UUW_Card::DetermineResultOfCardRelease()
{
	if (OnCardWidgetEvent.IsBound())
		OnCardWidgetEvent.Broadcast(CosGameTags::Event_Card_Drop, this);

	if (bActive)
	{
		bool bUseCardSuccess = CardActor->AttemptUseCard(MarkedActors, false, false, false);
		CardState = bUseCardSuccess ? ECardState::Activating : ECardState::InHand;
		SuggestedTransform = GetRenderTransform();
		HoverZoneBottom->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HoverZoneTop->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HandReference->HeldCard = nullptr;
		DeactivateCard();
	}
	else
	{
		ReturnCardToHand();
	}
}

void UUW_Card::ReturnCardToHand()
{
	HandReference->UpdateCardTransforms();
	ResetHoverZoom();
	CardState = ECardState::InHand;
	SetRenderScale(FVector2D(HandScale));

	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Slot);
	PanelSlot->SetZOrder(SuggestedZOrder);
	HandReference->HeldCard = nullptr;
	DeactivateCard();
	HoverZoneBottom->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	HoverZoneTop->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	FTimerHandle ReturnTimerDelay;
	GetWorld()->GetTimerManager().SetTimer(ReturnTimerDelay, [&]()
	{
		HoverZoneBottom->SetVisibility(ESlateVisibility::Visible);
		HoverZoneTop->SetVisibility(ESlateVisibility::Visible);
	}, 0.3f, false);
}

void UUW_Card::SetupBezierFollow(const FTransferAnim& NewTransferAnimData)
{
	TransferAnimData = NewTransferAnimData;
	CardState = ECardState::FollowBelzier;
}

void UUW_Card::FollowBezier()
{
	FWidgetTransform NewTransform;

	float SpeedAlpha = TransferAnimData.SpeedCurve->GetFloatValue(BezierAlpha);

	float Angle;
	FVector2D NewTranslation = UFunctionLibrary_Utility::GetPositionAndAngleAtBezierAlphaBetweenWidgets(this, SpeedAlpha, TransferAnimData.StartWidget, TransferAnimData.MidWidget,
	                                                                                                    TransferAnimData.EndWidget, Angle);

	NewTransform.Translation = NewTranslation + TransferAnimData.Offset;
	NewTransform.Scale = FVector2D(TransferAnimData.SpeedCurve->GetFloatValue(SpeedAlpha) * TransferAnimData.ScaleModifier);
	NewTransform.Shear = FVector2D::Zero();
	NewTransform.Angle = Angle * TransferAnimData.AngleModifier;
	SetRenderTransform(NewTransform);

	BezierAlpha = BezierAlpha + (UGameplayStatics::GetWorldDeltaSeconds(this) / TransferAnimData.Duration);

	if (BezierAlpha >= 1.0f)
	{
		if (OnRemoved.IsBound())
			OnRemoved.Broadcast(CardActor, this);
		RemoveFromParent();
	}
}

void UUW_Card::MoveToSuggestedTransform(bool bContinueWhenDone, const FWidgetTransform& InSuggestedTransform, float Speed)
{
	const FWidgetTransform& CurrentTransform = GetRenderTransform();

	if (UKismetMathLibrary::EqualEqual_Vector2DVector2D(CurrentTransform.Translation, InSuggestedTransform.Translation, 1.0f) &&
		FMath::IsNearlyEqual(CurrentTransform.Angle, InSuggestedTransform.Angle, 0.1) &&
		UKismetMathLibrary::EqualEqual_Vector2DVector2D(CurrentTransform.Scale, InSuggestedTransform.Scale))
	{
		return;
	}


	float NewAlpha = FMath::Min(1.0f, UGameplayStatics::GetWorldDeltaSeconds(this) * Speed);
	FVector NewTranslation = UKismetMathLibrary::VLerp(FVector(CurrentTransform.Translation.X, CurrentTransform.Translation.Y, 0),
	                                                   FVector(InSuggestedTransform.Translation.X, InSuggestedTransform.Translation.Y, 0),
	                                                   NewAlpha);

	FWidgetTransform NewRenderTransform;
	NewRenderTransform.Translation = FVector2D(NewTranslation.X, NewTranslation.Y);
	NewRenderTransform.Scale = UFunctionLibrary_Utility::LerpVector2D(CurrentTransform.Scale, InSuggestedTransform.Scale, NewAlpha);
	NewRenderTransform.Shear = FVector2D::Zero();
	NewRenderTransform.Angle = UKismetMathLibrary::Lerp(CurrentTransform.Angle, InSuggestedTransform.Angle, NewAlpha);
	SetRenderTransform(NewRenderTransform);
}

void UUW_Card::FollowMouseAndCheckActivation()
{
	const FVector2D& MousePositionOnViewport = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	MoveToSuggestedTransform(true, FWidgetTransform(MousePositionOnViewport, FVector2D(HoverScale), FVector2D::Zero(), 0.0f), MouseFollowSpeed);


	FVector2D PixelPosition, ViewportPosition;
	USlateBlueprintLibrary::LocalToViewport(this, HandReference->CardZone->GetTickSpaceGeometry(), HandReference->CardZone->GetRenderTransform().Translation,
	                                        PixelPosition, ViewportPosition);

	if (MousePositionOnViewport.Y < ViewportPosition.Y)
	{
		if (bActive)
			return;

		ActivateCardIfValid();
		return;
	}

	if (bActive)
	{
		DeactivateCard();
	}

	return;
}

void UUW_Card::RunTargetingMode()
{
	MoveToSuggestedTransform(true, FWidgetTransform(FVector2D(GetRenderTransform().Translation.X, TargetingModeYOffset),
	                                                FVector2D(HoverScale), FVector2D::Zero(), 0.0f), CardMoveSpeed);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	FHitResult HitResult;
	if (UGameplayStatics::GetPlayerController(this, 0)->GetHitResultUnderCursorForObjects(ObjectTypes, false, HitResult))
	{
		if (HitResult.GetActor()->Implements<UInterface_CardPuppet>())
		{
			AMinionBase* Minion;
			if (!IInterface_CardPuppet::Execute_GetMinionFromPuppet(HitResult.GetActor(), Minion))
				return;

			if (MarkedActors.Contains(Minion))
				return;

			HandleRunTargetingMode(HitResult.GetActor());
		}
	}
	else
	{
		HandleRunTargetingMode(nullptr);
	}
}

void UUW_Card::UnmarkMarkedActors()
{
	for (AActor* MarkedActor : MarkedActors)
	{
		if (IsValid(MarkedActor))
		{
			IInterface_CardTarget::Execute_UnMarkTarget(MarkedActor);
		}
	}

	MarkedActors.Empty();

	IInterface_TargetArrow::Execute_UpdateTargetArrow(TargetArrowReference, CardState == ECardState::Targeting, FLinearColor(1, 1, 1, 1), false, FVector2D::Zero(), false);
	ResetGlow();
}

void UUW_Card::HandleRunTargetingMode(AActor* TargetingActor)
{
	if (!TargetingActor)
		return;

	if (MarkedActors.Num() > 0)
	{
		UnmarkMarkedActors();
		DeactivateCard();
	}

	if (TargetingActor->Implements<UInterface_CardPuppet>())
	{
		AMinionBase* TargetMinion;
		if (IInterface_CardPuppet::Execute_GetMinionFromPuppet(TargetingActor, TargetMinion))
		{
			if (IInterface_CardTarget::Execute_AttemptMarkTarget(TargetMinion, CardActor->GetGameplayTags()))
			{
				IInterface_TargetArrow::Execute_UpdateTargetArrow(TargetArrowReference, true, GlowActive, false, FVector2D::Zero(), true);
				IInterface_CardWidget::Execute_ToggleCardGlow(this, true, GlowActive);
				MarkedActors.Add(TargetMinion);
				ActivateCardIfValid();
			}
		}
	}
}

void UUW_Card::ActivateCardIfValid()
{
	if (bActive) return;

	FString FallMessage;
	bool bPlayable = CardActor->CheckIfPlayable(FallMessage);

	if (bPlayable && CardActor->IsInHand())
	{
		bActive = true;
		if (CardActor->GetCardTargeted(ECardDataType::Hand)) return;

		IInterface_CardWidget::Execute_ToggleCardGlow(this, true, GlowActive);

		const TArray<AActor*>& ValidTargets = CardActor->FindAllPotentialTargets();
		for (AActor* ValidTarget : ValidTargets)
		{
			if (IInterface_CardTarget::Execute_MarkTarget(ValidTarget))
			{
				MarkedActors.Add(ValidTarget);
			}
		}
	}
	else
	{
		UFunctionLibrary_Utility::SendScreenLogMessage(this, FText::FromString(FallMessage), FColor(1, 0, 0));
		if (OnCardWidgetEvent.IsBound())
			OnCardWidgetEvent.Broadcast(CosGameTags::Event_Card_Drop, this);
		ReturnCardToHand();
	}
}

void UUW_Card::DeactivateCard()
{
	bActive = false;
	ResetGlow();
	UnmarkMarkedActors();

	FVector2D SourcePoint = GetRenderTransform().Translation + FVector2D(50.0f, -150.0f);
	IInterface_TargetArrow::Execute_UpdateTargetArrow(TargetArrowReference, CardState == ECardState::Targeting, FLinearColor(1, 1, 1, 1), false, SourcePoint, false);
}

void UUW_Card::ResetGlow()
{
	FString FallMessage;
	bool bPlayable = CardActor->CheckIfPlayable(FallMessage);
	IInterface_CardWidget::Execute_ToggleCardGlow(this, bPlayable, GlowValid);
}

void UUW_Card::BindToMouseRelease()
{
	MousePositionOnClick = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	DelayBindToMouseRelease();
}

void UUW_Card::UpdateOnCardUpdate()
{
	IInterface_CardWidget::Execute_UpdateCardWidget(CardVisual, CardActor);
	IInterface_CardWidget::Execute_AnimateCardNotify(CardVisual, EUMGSequencePlayMode::Forward);
}

void UUW_Card::DelayBindToMouseRelease()
{
	FVector2D MousePositionOnViewport = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	if ((MousePositionOnViewport - MousePositionOnClick).Size() > 20.0f)
	{
		UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(this, CosGameTags::Event_MouseRelease);
	}
	else
	{
		FTimerHandle BindTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(BindTimerHandle, this, &UUW_Card::DelayBindToMouseRelease, 0.1f, false);
	}
}

void UUW_Card::AnimateAutoPlay_Implementation()
{
	HoverZoneBottom->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	HoverZoneTop->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	CardMoveSpeed = AutoPlayMoveSpeed;
	FVector2D ViewPortPosition = HandReference->GetAnchorWidgetPosition(CosGameTags::Event_Action_AutoPlay);
	SuggestedTransform.Translation = ViewPortPosition;
	SuggestedTransform.Scale = FVector2D(HoverScale);

	FTimerHandle AnimateTimerDelay;
	GetWorld()->GetTimerManager().SetTimer(AnimateTimerDelay, [&]()
	{
		PlayAnimation(Activate);
		UUW_Card* WBPCardCDO = GetClass()->GetDefaultObject<UUW_Card>();
		CardMoveSpeed = WBPCardCDO->CardMoveSpeed;

		FTimerHandle ActivateTimerDelay;
		float ActivateDuration = Activate->GetEndTime() + 0.2f;
		GetWorld()->GetTimerManager().SetTimer(ActivateTimerDelay, [&]()
		{
			HoverZoneBottom->SetVisibility(ESlateVisibility::Visible);
			HoverZoneTop->SetVisibility(ESlateVisibility::Visible);
		}, ActivateDuration, false);
	}, 0.9f, false);
}

void UUW_Card::ToggleCardGlow_Implementation(bool bEnable, FLinearColor Color)
{
	IInterface_CardWidget::Execute_ToggleCardGlow(CardVisual, bEnable, Color);
}

void UUW_Card::UpdateCardWidget_Implementation(ACardBase* InCardActor)
{
	IInterface_CardWidget::Execute_UpdateCardWidget(CardVisual, InCardActor);
}

void UUW_Card::RequestDesiredTransformUpdate_Implementation(FWidgetTransform DesiredTransform, bool bEnableMovement)
{
	if (CardState != ECardState::Activating)
	{
		SuggestedTransform = DesiredTransform;
	}
}

void UUW_Card::SuggestCardZOrder_Implementation(int32 ZOrder)
{
	SuggestedZOrder = ZOrder;
	if (CardState == ECardState::InHand && !bActive)
	{
		UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Slot);
		CanvasPanelSlot->SetZOrder(SuggestedZOrder);
	}
}

void UUW_Card::ForceCardZOrder_Implementation(int32 ZOrder)
{
	SuggestedZOrder = ZOrder;
	UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Slot);
	CanvasPanelSlot->SetZOrder(SuggestedZOrder);
}

void UUW_Card::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (EventTag == CosGameTags::Event_MouseRelease)
	{
		UFunctionLibrary_Event::UnBindEventFromGlobalDispatcherHub(this, CosGameTags::Event_MouseRelease);
		if (CallTags.HasTag(CosGameTags::Flag_Active))
		{
			DetermineResultOfCardRelease();
		}
		else
		{
			bActive = false;
			ReturnCardToHand();
		}
	}
}