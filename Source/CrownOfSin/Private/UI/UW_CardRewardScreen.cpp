#include "UI/UW_CardRewardScreen.h"

#include "CardSystem/CardBase.h"
#include "Components/HorizontalBox.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Libraries/AssetPath.h"
#include "UI/UW_CardListCard.h"
#include "Utilities/CosLog.h"

UUW_CardRewardScreen::UUW_CardRewardScreen(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), Image_81(nullptr), RewardBox(nullptr), SkipButton(nullptr), SkipOverlay(nullptr),
                                                                                          Title(nullptr),
                                                                                          FadeIn(nullptr),
                                                                                          LoopIndex(0)
{
	static ConstructorHelpers::FClassFinder<UUW_CardListCard> WBP_CardListCardClass_C(*AssetPath::Blueprint::WBP_CardListCard_C);
	if (WBP_CardListCardClass_C.Succeeded())
	{
		WBP_CardListCardClass = WBP_CardListCardClass_C.Class;
	}
	else
	{
		COS_LOG_ERROR(TEXT("WBP_CardListCardClass를 찾지 못했습니다"));
	}
}

void UUW_CardRewardScreen::UpdateRewardScreen(const TArray<ACardBase*>& AvailableCards, const FText& TitleText, bool bIsSkipAllowed)
{
	COS_IF_CHECK_VOID(WBP_CardListCardClass, TEXT("WBP_CardListCardClass를 설정해주세요"));

	// 스킵 버튼 오버레이의 가시성 설정
	SkipOverlay->SetVisibility(bIsSkipAllowed ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	// 보상 상자 초기화
	RewardBox->ClearChildren();

	// 타이틀 텍스트 설정
	Title->SetText(TitleText);

	// UI를 표시
	SetVisibility(ESlateVisibility::Visible);

	// 카드 목록 위젯을 보상 상자에 추가
	for (int32 i = 0; i < AvailableCards.Num(); i++)
	{
		if (i >= RewardOptionCount)
		{
			break; // 보상 카드 수를 초과하지 않도록 제한
		}

		// 카드 위젯 생성
		UUW_CardListCard* CardWidget = Cast<UUW_CardListCard>(CreateWidget(GetWorld(), WBP_CardListCardClass));

		// 위젯에 카드 액터 설정
		CardWidget->CardActor = AvailableCards[i];

		// 보상 상자에 위젯 추가
		RewardBox->AddChild(CardWidget);

		// 카드 클릭 이벤트 바인딩
		CardWidget->OnCardClicked.AddDynamic(this, &UUW_CardRewardScreen::ReturnReward);
	}

	// 카드 옵션을 보여주는 블루프린트 함수 호출
	ShowCardOptions();
}

void UUW_CardRewardScreen::ReturnReward(UUW_CardListCard* SelectedCard, ACardBase* SelectedCardActor)
{
	// 보상 카드 목록에서 모든 위젯을 순회하면서 상태 변경
	for (UWidget* Widget : RewardBox->GetAllChildren())
	{
		UUW_CardListCard* CastCardListCard = Cast<UUW_CardListCard>(Widget);
		if (!CastCardListCard) continue;

		// 카드를 히트 테스트 비활성화 및 애니메이션 재생
		CastCardListCard->SetVisibility(ESlateVisibility::HitTestInvisible);
		CastCardListCard->bBlockHoverAnim = true;
		CastCardListCard->PlayAnimation(SelectedCard != CastCardListCard ? SelectedCard->FadeOut : SelectedCard->Select);
	}

	GetWorld()->GetTimerManager().SetTimer(RewardCompletionHandle, FTimerDelegate::CreateLambda([this, SelectedCardActor]()
	{
		// 첫 번째 단계: 애니메이션 재생
		PlayAnimationReverse(FadeIn);

		// 두 번째 단계: 0.1초 뒤에 호출할 작업 예약
		GetWorld()->GetTimerManager().SetTimer(ReturnCompletionHandle, FTimerDelegate::CreateLambda([this, SelectedCardActor]()
		{
			SetVisibility(ESlateVisibility::Collapsed);
			if (OnReturnSelectedCardInRewardScreen.IsBound())
			{
				OnReturnSelectedCardInRewardScreen.Broadcast(false, SelectedCardActor);
			}

			// 두 번째 타이머 클리어
			GetWorld()->GetTimerManager().ClearTimer(ReturnCompletionHandle);
		}), 0.1f, false);

		// 첫 번째 타이머 클리어
		GetWorld()->GetTimerManager().ClearTimer(RewardCompletionHandle);
	}), 0.4f, false);
}
