#include "UI/UW_CardHand.h"


#include "Blueprint/SlateBlueprintLibrary.h"
#include "CardSystem/CardBase.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Interfaces/Widget/Interface_CardWidget.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Utilities/CosGameplayTags.h"
#include "UI/UW_Anchor.h"
#include "UI/UW_Card.h"
#include "UI/UW_CardVisual.h"
#include "UI/UW_HandSelect.h"
#include "Utilities/CosLog.h"

UUW_CardHand::UUW_CardHand(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), HandCanvas(nullptr), CardZone(nullptr), WBP_HandAnchor(nullptr), WBP_PlayAnchor(nullptr),
                                                                          WBP_ReshuffleAnchor(nullptr),
                                                                          WBP_HandSelect(nullptr), WBP_Bezier(nullptr), CurrentEventCard(nullptr), HeldCard(nullptr)
{
}

void UUW_CardHand::NativeConstruct()
{
	Super::NativeConstruct();

	// 앵커 위젯을 초기화하여 등록
	TMap<FGameplayTag, UUserWidget*> AnchorWidgetMap;
	AnchorWidgetMap.Add(CosGameTags::Pile_Hand, WBP_HandAnchor);
	AnchorWidgetMap.Add(CosGameTags::Event_Action_AutoPlay, WBP_PlayAnchor);
	AnchorWidgetMap.Add(CosGameTags::Event_Action_GenerateCard, WBP_PlayAnchor);

	// 새로 생성된 앵커 위젯 맵을 등록하는 함수 호출
	AddAnchorWidgets(AnchorWidgetMap);

	// 주기적으로 카드의 변환을 업데이트하도록 설정
	UpdatePeriodically();

	// 이벤트 태그들을 컨테이너에 추가
	FGameplayTagContainer GameplayEventTags;
	GameplayEventTags.AddTag(CosGameTags::Event_Action_AutoPlay);
	GameplayEventTags.AddTag(CosGameTags::Event_Action_FlickCard);
	GameplayEventTags.AddTag(CosGameTags::Event_Action_GenerateCard);
	GameplayEventTags.AddTag(CosGameTags::Event_Action_ResourceChange);
	GameplayEventTags.AddTag(CosGameTags::Event_Action_UseCard);
	GameplayEventTags.AddTag(CosGameTags::Event_Action_Victory);
	GameplayEventTags.AddTag(CosGameTags::Event_CardSelectionMode);
	GameplayEventTags.AddTag(CosGameTags::Event_GameOver);
	GameplayEventTags.AddTag(CosGameTags::Event_Victory);

	// 각 이벤트 태그에 대해 글로벌 이벤트 디스패처에 바인딩
	for (const FGameplayTag& EventTag : GameplayEventTags.GetGameplayTagArray())
	{
		UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(this, EventTag);
	}
}

void UUW_CardHand::AddAnchorWidgets(const TMap<FGameplayTag, UUserWidget*>& AnchorWidgetMap)
{
	// 전달된 앵커 위젯 맵을 순회하면서 AnchorWidgets 맵에 추가
	for (const TTuple<FGameplayTag, UUserWidget*>& InAnchorWidget : AnchorWidgetMap)
	{
		AnchorWidgets.Add(InAnchorWidget);
	}
}

void UUW_CardHand::UpdatePeriodically()
{
	// 즉시 카드 변환 업데이트
	UpdateCardTransforms();

	// 타이머를 설정하여 주기적으로 HandleCardTransforms를 호출
	GetWorld()->GetTimerManager().SetTimer(UpdateTimerHandle, this, &UUW_CardHand::HandleCardTransforms, UpdatePeriod, true);
}

void UUW_CardHand::UpdateCardTransforms()
{
	// 카드 위젯의 마지막 인덱스 계산
	int32 LastCardIndex = CardWidgetsInHand.Num() - 1;

	// 손에 있는 각 카드 위젯의 변환값을 업데이트
	for (int CardIndex = 0; CardIndex < CardWidgetsInHand.Num(); ++CardIndex)
	{
		// 카드 위젯이 UInterface_CardWidget 인터페이스를 구현하지 않으면 건너뜀
		if (!CardWidgetsInHand[CardIndex]->Implements<UInterface_CardWidget>()) continue;

		// 변환 값을 담을 구조체 초기화
		FWidgetTransform DesiredTransform;

		// 기본 카드 스케일 및 변형 값 설정
		DesiredTransform.Scale = BaseCardScale;
		DesiredTransform.Shear = FVector2D::Zero();

		// 카드의 회전 각도 계산 (가운데를 기준으로 회전)
		DesiredTransform.Angle = (LastCardIndex * (CardAngleFromCenter * -1.0f)) + (CardIndex * (CardAngleFromCenter * 2.0f));

		// 손의 위치 계산
		const FVector2D& HandPosition = GetHandPosition();

		// 각 카드의 X 위치 및 Y 위치 계산
		float CardPosX = HandPosition.X + ((CardIndex * CardSpacing) - (LastCardIndex * (CardSpacing * 0.5f)));
		float CardPosY = FMath::Square(((CardIndex - (LastCardIndex * 0.5f)) * 2.0f)) + HandPosition.Y;
		DesiredTransform.Translation = FVector2D(CardPosX, CardPosY);

		// 카드 위젯에 변환값 요청 및 ZOrder 제안
		IInterface_CardWidget::Execute_RequestDesiredTransformUpdate(CardWidgetsInHand[CardIndex], DesiredTransform, true);
		IInterface_CardWidget::Execute_SuggestCardZOrder(CardWidgetsInHand[CardIndex], CardIndex + 1);
	}
}

void UUW_CardHand::EnableSelectionMode(int32 MaxSelectableCards, const FGameplayTagContainer& AllowedCardTags)
{
	// 선택 모드를 초기화하고, 최대 선택 가능 카드 수 및 유효 카드 태그를 설정
	WBP_HandSelect->InitializeEvent(MaxSelectableCards, this, AllowedCardTags);

	// 선택 모드를 활성화
	bSelectionMode = true;

	// 선택 위젯의 가시성을 보이도록 설정
	WBP_HandSelect->SetVisibility(ESlateVisibility::Visible);
}

void UUW_CardHand::HideCardsInHand()
{
	// 손에 있는 모든 카드 위젯에 대해 숨기기 애니메이션을 재생
	for (UUW_Card* CardInHand : CardWidgetsInHand)
	{
		PlayAnimation(CardInHand->HideDown);
	}
}

UUW_Card* UUW_CardHand::CreateAndSetupCardWidget(ACardBase* CardActor, const FWidgetTransform& StartTransform, bool bAddToHand)
{
	// 카드 클래스와 카드 시각화 클래스가 설정되지 않은 경우 오류 처리
	COS_IF_CHECK(WBP_CardClass, TEXT("UUW_CardHand에서 WBP_CardClass를 설정해주세요"), nullptr);
	COS_IF_CHECK(WBP_CardVisualClass, TEXT("UUW_CardHand에서 WBP_CardVisualClass를 설정해주세요"), nullptr);

	// 카드 위젯을 생성하고 초기화
	UUW_Card* NewCardWidget = CreateWidget<UUW_Card>(GetWorld(), WBP_CardClass);
	NewCardWidget->CardActor = CardActor;
	NewCardWidget->HandReference = this;
	NewCardWidget->TargetArrowReference = WBP_Bezier;
	NewCardWidget->CardVisualClass = WBP_CardVisualClass;

	// 위젯을 HandCanvas에 추가
	HandCanvas->AddChild(NewCardWidget);

	// 새 카드 위젯의 캔버스 슬롯 설정
	UCanvasPanelSlot* NewCardWidgetSlot = Cast<UCanvasPanelSlot>(NewCardWidget->Slot);
	NewCardWidgetSlot->SetSize(CardSizeInHand);
	NewCardWidgetSlot->SetAlignment(FVector2D(0.5f, 0.5f));

	// 카드 위젯 초기 설정
	NewCardWidget->ResetGlow();
	NewCardWidget->SetRenderTransform(StartTransform);

	// 위젯을 손에 추가할 경우 처리
	if (bAddToHand)
	{
		CardWidgets.Add(CurrentEventCard, NewCardWidget);
		CardWidgetsInHand.Add(NewCardWidget);
		BindHandToCardWidgetEvents(NewCardWidget);
	}

	// 생성된 카드 위젯 반환
	return NewCardWidget;
}

void UUW_CardHand::TransferCardAction(ACardBase* InCard, FGameplayTag SourcePile, FGameplayTag TargetPile)
{
	// 현재 이벤트 카드로 설정
	CurrentEventCard = InCard;

	// 카드가 드로우 더미에서 손으로 이동할 경우
	if (SourcePile == CosGameTags::Pile_Draw)
	{
		if (TargetPile == CosGameTags::Pile_Hand)
		{
			FGameplayTag StartAnchor = CosGameTags::Event_Action_Draw;

			// 드로우 애니메이션 처리
			HandleTransferCardAction(StartAnchor);
		}
		return;
	}

	// 카드가 손에서 다른 더미로 이동할 경우
	if (SourcePile == CosGameTags::Pile_Hand)
	{
		if (CardWidgets.Contains(CurrentEventCard))
		{
			CardWidgetsInHand.Remove(CardWidgets[CurrentEventCard]); // 손에서 제거
			CardWidgets[CurrentEventCard]->AnimateCardEvent(TargetPile); // 이동 애니메이션 실행
		}
		return;
	}

	// 그 외의 경우 처리 (기본 처리)
	if (CardWidgets.Contains(CurrentEventCard))
	{
		// 목표 더미가 손일 경우
		if (TargetPile == CosGameTags::Pile_Hand)
		{
			CardWidgetsInHand.Add(CardWidgets[CurrentEventCard]); // 손에 카드 추가
			CardWidgets[CurrentEventCard]->bActive = false;
			CardWidgets[CurrentEventCard]->bVisuallyInHand = true;
			UpdateCardTransforms(); // 카드 변환 업데이트
		}
		else
		{
			CardWidgets[CurrentEventCard]->AnimateCardEvent(TargetPile); // 목표 더미로 이동 애니메이션 실행
		}
	}
	else
	{
		// 목표 더미가 손일 경우 카드 위젯 생성
		if (TargetPile == CosGameTags::Pile_Hand)
		{
			FGameplayTag StartAnchor = SourcePile;
			HandleTransferCardAction(StartAnchor);
		}
	}
}

FVector2D UUW_CardHand::GetHandPosition()
{
	FVector2D ViewportPosition, PixelPosition;

	// 손 앵커 위젯의 위치를 뷰포트 좌표로 변환
	USlateBlueprintLibrary::LocalToViewport(this,
	                                        WBP_HandAnchor->GetTickSpaceGeometry(),
	                                        WBP_HandAnchor->GetRenderTransform().Translation,
	                                        PixelPosition,
	                                        ViewportPosition);
	return ViewportPosition;
}

FVector2D UUW_CardHand::GetOffsetDrawPosition()
{
	return DrawPosition + DrawOffset;
}

FVector2D UUW_CardHand::GetAnchorWidgetPosition(FGameplayTag AnchorWidget)
{
	// 앵커 위젯 맵에 해당 태그가 존재하는지 확인
	if (AnchorWidgets.Contains(AnchorWidget))
	{
		FVector2D PixelPosition, ViewportPosition;

		// 앵커 위젯의 화면 좌표 계산
		USlateBlueprintLibrary::LocalToViewport(this,
		                                        AnchorWidgets[AnchorWidget]->GetTickSpaceGeometry(),
		                                        AnchorWidgets[AnchorWidget]->GetRenderTransform().Translation,
		                                        PixelPosition,
		                                        ViewportPosition);

		// 앵커 오프셋을 적용한 뷰포트 위치 반환
		return ViewportPosition + AnchorOffset;
	}

	// 해당하는 앵커 위젯이 없으면 빈 FVector2D 반환
	return {};
}

void UUW_CardHand::HandleCardTransforms()
{
	// 카드 변환 업데이트
	UpdateCardTransforms();
}

void UUW_CardHand::BindHandToCardWidgetEvents(UUW_Card* InCardWidget)
{
	InCardWidget->OnRemoved.AddDynamic(this,&UUW_CardHand::UUW_CardHand::RemoveCard);
	InCardWidget->OnCardWidgetEvent.AddDynamic(this,&UUW_CardHand::UUW_CardHand::ResolveCardWidgetEvent);
}

void UUW_CardHand::RemoveCard(ACardBase* RemovedCard, UUserWidget* RemovedWidget)
{
	// 제거할 카드가 위젯 맵에 존재하는지 확인
	if (CardWidgets.Contains(RemovedCard))
	{
		// 제거할 위젯이 해당 카드의 위젯과 일치하면 맵에서 제거
		if (CardWidgets[RemovedCard] == RemovedWidget)
		{
			CardWidgets.Remove(RemovedCard);
		}
	}
}

void UUW_CardHand::ResolveCardWidgetEvent(FGameplayTag InEvent, UUW_Card* InCardWidget)
{
	// 카드가 선택된 이벤트인 경우
	if (InEvent == CosGameTags::Event_Card_Grab)
	{
		// 카드 선택 모드일 때
		if (bSelectionMode)
		{
			// 카드를 선택 시도 후 성공하면 카드를 숨기고 콜백을 설정
			if (WBP_HandSelect->AttemptAddCard(InCardWidget->CardActor, InCardWidget))
			{
				InCardWidget->SetVisibility(ESlateVisibility::Hidden);
				WBP_HandSelect->OnReturnCard.AddDynamic(this, &UUW_CardHand::ReturnCardFromHandSelect);
				WBP_HandSelect->OnConfirmCards.AddDynamic(this, &UUW_CardHand::ConfirmSelectedCards);
			}
			return;
		}

		// 일반 모드에서 카드가 손에 들어왔음을 표시
		bCardInHand = true;
		return;
	}


	// 카드가 놓인 이벤트인 경우
	if (InEvent == CosGameTags::Event_Card_Drop)
	{
		bCardInHand = false;
		return;
	}
}

void UUW_CardHand::ReturnCardFromHandSelect(UWidget* PairedCardWidget)
{
	// 카드를 다시 손으로 돌려줄 때 가시성 설정
	PairedCardWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UUW_CardHand::ConfirmSelectedCards(const TArray<ACardBase*>& SelectedCards)
{
	// 선택된 카드가 있으면 바인딩된 델리게이트에 브로드캐스트
	if (OnReturnSelectedCards.IsBound())
	{
		OnReturnSelectedCards.Broadcast(SelectedCards);
	}

	// 카드 선택 UI 숨기기
	WBP_HandSelect->SetVisibility(ESlateVisibility::Collapsed);

	// 선택 모드를 종료
	bSelectionMode = false;

	// 카드 변환 업데이트
	UpdateCardTransforms();
}

void UUW_CardHand::HandleTransferCardAction(FGameplayTag StartAnchor)
{
	// 앵커 위치를 가져와서 카드 위젯을 생성
	FVector2D AnchorPosition = GetAnchorWidgetPosition(StartAnchor);
	CreateAndSetupCardWidget(CurrentEventCard, FWidgetTransform(AnchorPosition, FVector2D::Zero(), FVector2D::Zero(), 0.0f), true);

	// 카드 변환 업데이트
	UpdateCardTransforms();

	// 글로벌 디스패처에 드로우 이벤트 호출
	UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_Action_Draw, this, nullptr);

	// 생성된 카드가 손에 있는지 확인 후 시각적 업데이트
	if (CardWidgets.Contains(CurrentEventCard))
	{
		CardWidgets[CurrentEventCard]->bVisuallyInHand = true;
	}
}

void UUW_CardHand::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	// 현재 실행 중인 이벤트 태그를 저장
	CurrentEventTag = EventTag;

	// 이벤트를 호출한 객체가 카드일 경우 처리
	if (ACardBase* CallingCard = Cast<ACardBase>(CallingObject))
	{
		// 현재 이벤트 카드를 설정
		CurrentEventCard = CallingCard;

		// 카드 생성 이벤트 처리
		if (CurrentEventTag == CosGameTags::Event_Action_GenerateCard)
		{
			// 카드 생성에 필요한 앵커 위치를 계산
			FVector2D AnchorViewportPosition = GetAnchorWidgetPosition(CosGameTags::Event_Action_GenerateCard);

			// 카드 위젯 생성 및 설정
			UUW_Card* CreatedCardWidget = CreateAndSetupCardWidget(CurrentEventCard, FWidgetTransform(AnchorViewportPosition, FVector2D::Zero(), FVector2D::Zero(), 0.0f), false);
			CreatedCardWidget->bActive = true;

			// 카드 위젯을 맵에 추가
			CardWidgets.Add(CurrentEventCard, CreatedCardWidget);

			// ZOrder 강제 설정
			IInterface_CardWidget::Execute_ForceCardZOrder(CreatedCardWidget, GenerateZOrder);

			// 카드 위젯의 변환 값 설정
			FWidgetTransform DesiredTransform;
			DesiredTransform.Translation = AnchorViewportPosition;
			DesiredTransform.Scale = FVector2D(1.0f, 1.0f);
			DesiredTransform.Shear = FVector2D::Zero();
			DesiredTransform.Angle = 0.0f;

			// 카드 위젯에 변환 요청
			IInterface_CardWidget::Execute_RequestDesiredTransformUpdate(CreatedCardWidget, DesiredTransform, true);

			// 카드 위젯에 관련된 이벤트 바인딩
			BindHandToCardWidgetEvents(CreatedCardWidget);

			return;
		}

		// 카드가 손으로 반환되는 이벤트 처리
		if (CurrentEventTag == CosGameTags::Event_Action_ReturnCardToHand)
		{
			if (CardWidgets.Contains(CurrentEventCard))
			{
				// 손에 카드 위젯 추가
				CardWidgetsInHand.Add(CardWidgets[CurrentEventCard]);

				// 일정 시간 후 카드 변환 업데이트
				FTimerHandle ReturnToTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(ReturnToTimerHandle, [&]()
				{
					CardWidgets[CurrentEventCard]->ReturnCardToHand();
					UpdateCardTransforms();
				}, 0.5f, false);
			}

			return;
		}

		// 카드가 자동 재생, 사용 또는 카드 선택 모드일 때 처리
		if (CurrentEventTag == CosGameTags::Event_Action_FlickCard || CurrentEventTag == CosGameTags::Event_Action_AutoPlay
			|| CurrentEventTag == CosGameTags::Event_Action_UseCard || CurrentEventTag == CosGameTags::Event_CardSelectionMode)
		{
			if (CardWidgets.Contains(CurrentEventCard))
			{
				// 손에서 카드 제거 및 애니메이션 실행
				CardWidgetsInHand.Remove(CardWidgets[CurrentEventCard]);
				CardWidgets[CurrentEventCard]->AnimateCardEvent(CurrentEventTag);
			}
			return;
		}
	}
	else
	{
		// 게임 승리 또는 게임 오버 이벤트 처리
		if (CurrentEventTag == CosGameTags::Event_Victory || CurrentEventTag == CosGameTags::Event_GameOver)
		{
			bEnabled = false;
			return;
		}

		// 자원 변경 이벤트 처리
		if (CurrentEventTag == CosGameTags::Event_Action_ResourceChange)
		{
			// 손에 있는 카드들의 시각적 상태를 초기화
			for (UUW_Card* CardInHand : CardWidgetsInHand)
			{
				CardInHand->ResetGlow();
			}

			return;
		}

		// 승리 시 손에 있는 카드를 숨김
		if (CurrentEventTag == CosGameTags::Event_Action_Victory)
		{
			HideCardsInHand();
		}
	}
}