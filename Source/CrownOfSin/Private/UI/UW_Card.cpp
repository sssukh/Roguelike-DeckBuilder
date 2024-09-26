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

	// CardActor가 유효하지 않으면 초기화 작업을 중단
	COS_IF_CHECK(CardActor, TEXT("CardActor가 유효하지 않습니다."));

	// 카드 UI 위젯 업데이트
	IInterface_CardWidget::Execute_UpdateCardWidget(CardVisual, CardActor);

	// 카드 발광(Glow) 상태 초기화
	ResetGlow();

	// 카드 수정 이벤트 바인딩
	CardActor->OnThisCardModified.AddDynamic(this, &UUW_Card::UpdateOnCardUpdate);

	// 카드 툴팁을 추가
	for (const FDataTableRowHandle& CardTooltip : CardActor->GetCardTooltips(ECardDataType::Hand))
	{
		constexpr int32 TooltipValue = 0;
		WBP_TooltipList->AddToolTipFromData(CardTooltip, TooltipValue);
	}
}

void UUW_Card::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	switch (CardState)
	{
	case ECardState::InHand:
	case ECardState::Activating:
		MoveToSuggestedTransform(false, SuggestedTransform, CardMoveSpeed);
		break;

	case ECardState::FollowMouse:
		FollowMouseAndCheckActivation();
		break;

	case ECardState::Targeting:
		RunTargetingMode();
		break;

	case ECardState::FollowBelzier:
		FollowBezier();
		break;

	default:
		// CardState가 예상하지 않은 값일 경우 처리할 로직
		break;
	}
}

void UUW_Card::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 손에 들고 있는 카드가 없을 때만 진행
	if (!IsValid(HandReference->HeldCard))
	{
		// 카드가 손에 있고 비활성 상태이며 시각적으로 손에 있는 상태일 경우
		if (CardState == ECardState::InHand && !bActive && CardActor->IsInHand() && bVisuallyInHand)
		{
			// 카드 상태를 MouseOver로 변경
			CardState = ECardState::MouseOver;

			// CanvasPanelSlot으로 Z-Order 설정
			if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
			{
				CanvasSlot->SetZOrder(HoverZOrder);
			}

			// 필요한 Y 좌표를 계산하여 카드의 위치를 조정
			const FWidgetTransform& CurrentRenderTransform = GetRenderTransform();
			FWidgetTransform NewRenderTransform;

			// 손의 위치 기반으로 새로운 Y 좌표 계산
			float HandAnchorY = HandReference->GetAnchorWidgetPosition(CosGameTags::Pile_Hand).Y;
			float AdjustedYPosition = HandAnchorY + ((HoverOffsetY * HoverOffsetJumpPropY) + UE5HoverOffset);
			NewRenderTransform.Translation = FVector2D(CurrentRenderTransform.Translation.X, AdjustedYPosition);
			NewRenderTransform.Scale = FVector2D(HoverScale);
			NewRenderTransform.Shear = FVector2D::ZeroVector;
			NewRenderTransform.Angle = 0.0f;

			// RenderTransform을 설정
			SetRenderTransform(NewRenderTransform);

			// 마우스 오버 시 추가 동작 실행
			MoveOnMouseOver();
		}
	}
}

void UUW_Card::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	// 카드가 마우스 오버 상태였을 때만 처리
	if (CardState == ECardState::MouseOver)
	{
		// 손에 들고 있는 카드가 없을 때만 처리
		if (!IsValid(HandReference->HeldCard))
		{
			// 카드 상태를 InHand로 되돌림
			CardState = ECardState::InHand;

			// 카드 크기를 원래대로 설정
			SetRenderScale(FVector2D(HandScale));

			// CanvasPanelSlot을 통해 Z-Order 복구
			if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Slot))
			{
				CanvasPanelSlot->SetZOrder(SuggestedZOrder);
			}

			// Hover 상태를 해제 (줌 및 애니메이션 해제)
			ResetHoverZoom();

			// 타이머를 사용하여 HoverZoneTop의 가시성 조정
			GetWorld()->GetTimerManager().ClearTimer(MouseLeaveTimerHandle); // 타이머 초기화
			GetWorld()->GetTimerManager().SetTimer(MouseLeaveTimerHandle, [&]()
			{
				// HoverZoneTop의 가시성을 HoverZoneBottom과 동일하게 설정
				HoverZoneTop->SetVisibility(HoverZoneBottom->GetVisibility());
			}, 0.3f, false);
		}
	}
}

FReply UUW_Card::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	 return FReply::Handled();
	// // 카드 잡기 이벤트 브로드캐스트
	// if (OnCardWidgetEvent.IsBound())
	// {
	// 	OnCardWidgetEvent.Broadcast(CosGameTags::Event_Card_Grab, this);
	// }
	//
	// // 카드가 마우스 오버 상태이고, 선택 모드가 아닌 경우
	// if (CardState == ECardState::MouseOver && !HandReference->bSelectionMode && HandReference->bEnabled)
	// {
	// 	ATurnManager* TurnManager = Cast<ATurnManager>(UGameplayStatics::GetActorOfClass(this, ATurnManager::StaticClass()));
	//
	// 	// 전투가 종료된 경우 카드 내려놓기 이벤트 처리
	// 	if (TurnManager->bCombatOver)
	// 	{
	// 		if (OnCardWidgetEvent.IsBound())
	// 		{
	// 			OnCardWidgetEvent.Broadcast(CosGameTags::Event_Card_Drop, this);
	// 		}
	// 		return FReply::Handled();
	// 	}
	//
	// 	// 마우스 릴리스 이벤트 바인딩
	// 	BindToMouseRelease();
	// 	HandReference->HeldCard = this;
	// 	bHovering = false;
	//
	// 	// 카드의 Z-Order 설정
	// 	if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(Slot))
	// 	{
	// 		CanvasPanelSlot->SetZOrder(HoverZOrder);
	// 	}
	//
	// 	WBP_TooltipList->SetVisibility(ESlateVisibility::Collapsed);
	//
	// 	// 카드가 타겟팅 상태가 아니라면
	// 	if (!CardActor->GetCardTargeted(ECardDataType::Hand))
	// 	{
	// 		// 카드 상태를 마우스 추적 모드로 전환
	// 		CardState = ECardState::FollowMouse;
	// 		return FReply::Handled();
	// 	}
	//
	// 	FString FailMessage;
	// 	bool bPlayable = CardActor->CheckIfPlayable(FailMessage);
	//
	// 	// 카드가 손에 있고 플레이 가능할 경우 타겟팅 모드 활성화
	// 	if (CardActor->IsInHand() && bPlayable)
	// 	{
	// 		CardState = ECardState::Targeting;
	// 		FVector2D SourcePoint = GetRenderTransform().Translation + FVector2D(0, -170.0f);
	// 		IInterface_TargetArrow::Execute_UpdateTargetArrow(TargetArrowReference, true, FLinearColor::White, true, SourcePoint, false);
	// 		return FReply::Handled();
	// 	}
	//
	// 	// 실패 메시지와 함께 카드 내려놓기 이벤트 처리
	// 	UFunctionLibrary_Utility::SendScreenLogMessage(this, FText::FromString(FailMessage), FColor(1, 0, 0));
	// 	if (OnCardWidgetEvent.IsBound())
	// 	{
	// 		OnCardWidgetEvent.Broadcast(CosGameTags::Event_Card_Drop, this);
	// 	}
	//
	// 	ReturnCardToHand();
	// 	return FReply::Handled();
	// }
	//
	//
	// // 그 외의 경우 카드 내려놓기 이벤트 처리
	// if (OnCardWidgetEvent.IsBound())
	// {
	// 	OnCardWidgetEvent.Broadcast(CosGameTags::Event_Card_Drop, this);
	// }
	// return FReply::Handled();
}

void UUW_Card::MoveOnMouseOver()
{
	// 이미 마우스 오버 상태가 아니라면 처리
	if (bMouseOverFlag)
	{
		bMouseOverFlag = false; // 마우스 오버 플래그를 false로 설정하여 중복 호출 방지

		// HoverMove 애니메이션 실행
		PlayAnimation(HoverMove);

		// 기존 타이머가 있으면 제거하고, 타이머를 설정
		GetWorld()->GetTimerManager().ClearTimer(HoverEffectTimerHandle); // 타이머 초기화
		GetWorld()->GetTimerManager().SetTimer(HoverEffectTimerHandle, [&]()
		{
			// HoverZoneTop의 가시성을 숨김으로 설정
			HoverZoneTop->SetVisibility(ESlateVisibility::Hidden);
		}, 0.1f, false);
	}
}

void UUW_Card::ResetHoverZoom()
{
	// 툴팁 리스트의 가시성을 숨김으로 설정
	WBP_TooltipList->SetVisibility(ESlateVisibility::Collapsed);

	// 마우스 오버 애니메이션을 반대로 재생
	ReverseAnimation(HoverMove);

	//플래그 설정
	bMouseOverFlag = true;
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
	// 카드 드롭 이벤트 브로드캐스트
	if (OnCardWidgetEvent.IsBound())
	{
		OnCardWidgetEvent.Broadcast(CosGameTags::Event_Card_Drop, this);
	}

	// 카드가 활성 상태인 경우
	if (bActive)
	{
		// 카드를 사용하려 시도
		const bool bUseCardSuccess = CardActor->AttemptUseCard(MarkedActors, false, false, false);

		// 카드 사용 성공 여부에 따라 상태 설정
		CardState = bUseCardSuccess ? ECardState::Activating : ECardState::InHand;

		SuggestedTransform = GetRenderTransform();

		// 카드 비활성화 및 참조 해제
		HoverZoneBottom->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HoverZoneTop->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		HandReference->HeldCard = nullptr;
		DeactivateCard();
	}
	// 카드가 비활성 상태인 경우
	else
	{
		ReturnCardToHand();
	}
}

void UUW_Card::ReturnCardToHand()
{
	// 카드의 변환 상태 및 시각적 요소를 초기화
	HandReference->UpdateCardTransforms();
	ResetHoverZoom();
	CardState = ECardState::InHand;
	SetRenderScale(FVector2D(HandScale));

	// Slot이 유효할 경우 ZOrder를 업데이트
	if (UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Slot))
	{
		PanelSlot->SetZOrder(SuggestedZOrder);
	}

	// 카드의 참조 해제
	HandReference->HeldCard = nullptr;

	// 카드 비활성화 처리
	DeactivateCard();

	// HoverZone 요소들을 숨김
	HoverZoneBottom->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	HoverZoneTop->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 0.3초 후 HoverZone을 다시 표시하는 타이머 설정
	FTimerHandle HoverZoneVisibilityTimer;
	GetWorld()->GetTimerManager().SetTimer(HoverZoneVisibilityTimer, [&]()
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

	// 베지어 곡선의 진행 상태를 가져옴
	float BezierProgress = TransferAnimData.SpeedCurve->GetFloatValue(BezierAlpha);

	// 베지어 곡선 상의 위치 및 각도 계산
	float Angle;
	FVector2D NewTranslation = UFunctionLibrary_Utility::GetPositionAndAngleAtBezierAlphaBetweenWidgets(
		this,
		BezierProgress,
		TransferAnimData.StartWidget,
		TransferAnimData.MidWidget,
		TransferAnimData.EndWidget,
		Angle
	);

	// 새로운 변환 상태 설정 (위치, 크기, 각도 등)
	NewTransform.Translation = NewTranslation + TransferAnimData.Offset;
	NewTransform.Scale = FVector2D(TransferAnimData.SpeedCurve->GetFloatValue(BezierProgress) * TransferAnimData.ScaleModifier);
	NewTransform.Shear = FVector2D::Zero();
	NewTransform.Angle = Angle * TransferAnimData.AngleModifier;
	SetRenderTransform(NewTransform);

	// 베지어 진행도를 업데이트 (프레임 기반이 아닌 타임 기반)
	BezierAlpha += FMath::Clamp(UGameplayStatics::GetWorldDeltaSeconds(this) / TransferAnimData.Duration, 0.0f, 1.0f);

	// 애니메이션이 끝났을 때 카드 제거
	if (BezierAlpha >= 1.0f)
	{
		if (OnRemoved.IsBound())
		{
			OnRemoved.Broadcast(CardActor, this);
		}

		// 후처리 콜백 등을 추가할 수 있음
		RemoveFromParent();
	}
}

void UUW_Card::MoveToSuggestedTransform(bool bContinueWhenDone, const FWidgetTransform& InSuggestedTransform, float Speed)
{
	const FWidgetTransform& CurrentTransform = GetRenderTransform();

	// 현재 변환 상태와 목표 변환 상태가 거의 같은지 확인
	if (UKismetMathLibrary::EqualEqual_Vector2DVector2D(CurrentTransform.Translation, InSuggestedTransform.Translation, 0.1f) &&
		FMath::IsNearlyEqual(CurrentTransform.Angle, InSuggestedTransform.Angle, 0.1f) &&
		UKismetMathLibrary::EqualEqual_Vector2DVector2D(CurrentTransform.Scale, InSuggestedTransform.Scale))
	{
		// bContinueWhenDone가 true일 때 추가 동작 수행 가능
		if (bContinueWhenDone)
		{
			// 추가 동작 (예: 애니메이션 완료 콜백) 처리
		}
		return;
	}

	// 부드러운 이동을 위한 알파 값 계산 (Lerp)
	float Alpha = FMath::Clamp(UGameplayStatics::GetWorldDeltaSeconds(this) * Speed, 0.0f, 1.0f);

	// Translation을 2D 벡터로 처리
	FVector2D InterpolatedTranslation = FMath::Lerp(CurrentTransform.Translation, InSuggestedTransform.Translation, Alpha);

	// 새로운 변환 상태를 설정
	FWidgetTransform NewRenderTransform;
	NewRenderTransform.Translation = InterpolatedTranslation;
	NewRenderTransform.Scale = UFunctionLibrary_Utility::LerpVector2D(CurrentTransform.Scale, InSuggestedTransform.Scale, Alpha);
	NewRenderTransform.Shear = FVector2D::Zero();
	NewRenderTransform.Angle = FMath::Lerp(CurrentTransform.Angle, InSuggestedTransform.Angle, Alpha);

	// 카드의 변환 상태 업데이트
	SetRenderTransform(NewRenderTransform);
}

void UUW_Card::FollowMouseAndCheckActivation()
{
	// 현재 마우스 위치를 가져옴
	const FVector2D& MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	// 마우스 위치에 따라 카드 이동
	MoveToSuggestedTransform(true, FWidgetTransform(MousePos, FVector2D(HoverScale), FVector2D::Zero(), 0.0f), MouseFollowSpeed);

	// 카드 영역의 위치를 계산
	FVector2D PixelPosition, CardZonePos;
	USlateBlueprintLibrary::LocalToViewport(this, HandReference->CardZone->GetTickSpaceGeometry(), HandReference->CardZone->GetRenderTransform().Translation, PixelPosition, CardZonePos);

	// 마우스 위치가 카드 영역보다 위에 있을 경우 카드 활성화 확인
	if (MousePos.Y < CardZonePos.Y)
	{
		if (!bActive)
		{
			ActivateCardIfValid(); // 카드 활성화
		}
	}
	else if (bActive)
	{
		DeactivateCard(); // 카드 비활성화
	}
}

void UUW_Card::RunTargetingMode()
{
	// 타겟팅 모드에서 카드의 위치를 이동시킴 (한 번만 호출되도록 변경 가능)
	MoveToSuggestedTransform(true, FWidgetTransform(FVector2D(GetRenderTransform().Translation.X, TargetingModeYOffset),
	                                                FVector2D(HoverScale), FVector2D::Zero(), 0.0f), CardMoveSpeed);

	// 타겟팅할 오브젝트 타입을 설정 (ECC_Pawn을 포함)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	// 마우스 커서 아래에 있는 오브젝트를 감지
	FHitResult HitResult;
	if (UGameplayStatics::GetPlayerController(this, 0)->GetHitResultUnderCursorForObjects(ObjectTypes, false, HitResult))
	{
		AActor* HitActor = HitResult.GetActor();

		// 감지된 오브젝트가 CardPuppet 인터페이스를 구현하는 경우
		if (HitActor && HitActor->Implements<UInterface_CardPuppet>())
		{
			AMinionBase* Minion;

			// 감지된 오브젝트에서 Minion 객체를 추출하고 이미 마킹된 타겟이 아니면 처리
			if (IInterface_CardPuppet::Execute_GetMinionFromPuppet(HitActor, Minion) && !MarkedActors.Contains(Minion))
			{
				HandleRunTargetingMode(HitActor); // 타겟팅 처리 함수 호출
			}
		}
	}
	else
	{
		HandleRunTargetingMode(nullptr); // 타겟팅 해제 처리
	}
}

void UUW_Card::UnmarkMarkedActors()
{
	// 마킹된 타겟들을 모두 해제
	for (AActor* MarkedActor : MarkedActors)
	{
		if (IsValid(MarkedActor))
		{
			IInterface_CardTarget::Execute_UnMarkTarget(MarkedActor);
		}
	}

	// 마킹된 타겟 목록을 비움
	MarkedActors.Empty();

	// 화살표 및 발광 상태를 업데이트
	IInterface_TargetArrow::Execute_UpdateTargetArrow(TargetArrowReference, CardState == ECardState::Targeting, FLinearColor::White, false, FVector2D::Zero(), false);
	ResetGlow();
}

void UUW_Card::HandleRunTargetingMode(AActor* TargetingActor)
{
	// 마킹된 타겟이 있는 경우 해제 및 카드 비활성화
	if (MarkedActors.Num() > 0)
	{
		UnmarkMarkedActors();
		DeactivateCard();
	}

	// 유효한 타겟이 없는 경우 반환
	if (!TargetingActor)
		return;

	// 타겟이 CardPuppet 인터페이스를 구현하는 경우
	if (TargetingActor->Implements<UInterface_CardPuppet>())
	{
		AMinionBase* MinionTarget;

		// 타겟에서 Minion을 추출하고 유효한 경우 처리
		if (IInterface_CardPuppet::Execute_GetMinionFromPuppet(TargetingActor, MinionTarget))
		{
			// 타겟을 마킹할 수 있는 경우 처리
			if (IInterface_CardTarget::Execute_AttemptMarkTarget(MinionTarget, CardActor->GetGameplayTags()))
			{
				// 타겟 화살표 및 카드 발광 처리
				IInterface_TargetArrow::Execute_UpdateTargetArrow(TargetArrowReference, true, GlowActive, false, FVector2D::Zero(), true);
				IInterface_CardWidget::Execute_ToggleCardGlow(this, true, GlowActive);

				// 마킹된 타겟 목록에 추가
				MarkedActors.Add(MinionTarget);

				// 카드가 유효한지 검증하고 활성화
				ActivateCardIfValid();
			}
		}
	}
}

void UUW_Card::ActivateCardIfValid()
{
	// 이미 활성화된 경우 바로 반환
	if (bActive) return;

	// 카드가 플레이 가능한지 확인
	FString ErrorMessage;
	bool bIsPlayable = CardActor->CheckIfPlayable(ErrorMessage);

	// 카드가 플레이 가능하고 손에 있을 때만 활성화
	if (bIsPlayable && CardActor->IsInHand())
	{
		bActive = true;

		// 타겟 설정 여부 확인 후 반환
		if (CardActor->GetCardTargeted(ECardDataType::Hand)) return;

		// 카드에 발광 효과 적용
		IInterface_CardWidget::Execute_ToggleCardGlow(this, true, GlowActive);

		// 모든 유효한 타겟들을 찾아 마킹
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
		// 카드가 플레이 불가능할 경우 메시지를 출력하고 카드를 손으로 되돌림
		UFunctionLibrary_Utility::SendScreenLogMessage(this, FText::FromString(ErrorMessage), FColor(1, 0, 0));

		if (OnCardWidgetEvent.IsBound())
		{
			OnCardWidgetEvent.Broadcast(CosGameTags::Event_Card_Drop, this);
		}

		ReturnCardToHand();
	}
}

void UUW_Card::DeactivateCard()
{
	bActive = false;

	// 마킹된 타겟들을 해제하고 발광 상태를 초기화
	UnmarkMarkedActors();

	ResetGlow();

	// 화살표 위치를 업데이트
	bool bVisibility = CardState == ECardState::Targeting;
	FVector2D SourcePoint = GetRenderTransform().Translation + FVector2D(50.0f, -150.0f);
	IInterface_TargetArrow::Execute_UpdateTargetArrow(TargetArrowReference, bVisibility, FLinearColor::White, false, SourcePoint, false);
}

void UUW_Card::ResetGlow()
{
	FString FallMessage;
	bool bPlayable = CardActor->CheckIfPlayable(FallMessage);

	// 카드 발광 상태를 플레이 가능 여부에 따라 변경
	IInterface_CardWidget::Execute_ToggleCardGlow(this, bPlayable, GlowValid);
}

void UUW_Card::BindToMouseRelease()
{
	// 현재 마우스 클릭 위치 저장
	MousePositionOnClick = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	// 마우스 위치를 추적하여 릴리스 이벤트를 바인딩하는 함수 호출
	DelayBindToMouseRelease();
}

void UUW_Card::DelayBindToMouseRelease()
{
	// 현재 마우스 위치를 가져옴
	FVector2D MousePositionOnViewport = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);

	// 마우스 이동 거리가 20 이상인 경우 마우스 릴리스 이벤트를 글로벌 디스패처에 바인딩
	if ((MousePositionOnViewport - MousePositionOnClick).Size() > 20.0f)
	{
		UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(this, CosGameTags::Event_MouseRelease);
	}
	else
	{
		// 이동 거리가 충분하지 않으면 타이머로 다시 확인
		GetWorld()->GetTimerManager().ClearTimer(MouseReleaseBindTimerHandle); // 타이머 중복 방지
		GetWorld()->GetTimerManager().SetTimer(MouseReleaseBindTimerHandle, this, &UUW_Card::DelayBindToMouseRelease, 0.1f, false);
	}
}

void UUW_Card::UpdateOnCardUpdate()
{
	// 카드 UI를 갱신
	IInterface_CardWidget::Execute_UpdateCardWidget(CardVisual, CardActor);
	// 카드 업데이트 알림 애니메이션을 실행
	IInterface_CardWidget::Execute_AnimateCardNotify(CardVisual, EUMGSequencePlayMode::Forward);
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
	// 마우스 릴리스 이벤트 처리
	if (EventTag == CosGameTags::Event_MouseRelease)
	{
		// 마우스 릴리스 이벤트 바인딩 해제
		UFunctionLibrary_Event::UnBindEventFromGlobalDispatcherHub(this, CosGameTags::Event_MouseRelease);

		// 활성화 태그가 있는 경우 카드 릴리스 결과 처리
		if (CallTags.HasTag(CosGameTags::Flag_Active))
		{
			DetermineResultOfCardRelease();
		}
		// 활성화 태그가 없는 경우 카드를 손으로 되돌림
		else
		{
			bActive = false;
			ReturnCardToHand();
		}
	}
	// 다른 이벤트 처리 가능성 확장
	else
	{
		// 필요한 경우 추가적인 이벤트 처리 로직을 작성할 수 있음
	}
}
