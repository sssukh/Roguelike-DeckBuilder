#include "UI/UW_HandSelect.h"

#include "CardSystem/CardBase.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "UI/UW_CardListCard.h"
#include "UI/UW_Card.h"
#include "Utilities/CosLog.h"

UUW_HandSelect::UUW_HandSelect(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), CardSelectionBox(nullptr), CommandText(nullptr), ConfirmButton(nullptr),
                                                                              EndTurnOverlay(nullptr), Image_81(nullptr)
{
}

void UUW_HandSelect::NativeConstruct()
{
	Super::NativeConstruct();

	ConfirmButton->OnClicked.AddDynamic(this, &UUW_HandSelect::ConfirmButtonOnClicked);
}

void UUW_HandSelect::InitializeEvent(int32 CardCount, UUW_CardHand* InSourceHand, const FGameplayTagContainer& InValidCardTags)
{
	// 카드 목록 위젯 클래스가 설정되어 있는지 확인
	COS_IF_CHECK(WBP_CardListCardClass, TEXT("UUW_HandSelect클래스에서 WBP_CardListCardClass를 설정해주세요."));

	// 카드 수가 0 이하일 경우 함수를 종료
	if (CardCount <= 0) return;

	// 카드 선택 박스의 자식 위젯 초기화 (기존 카드 제거)
	CardSelectionBox->ClearChildren();

	// 소스 핸드와 유효 카드 태그를 설정
	SourceHand = InSourceHand;
	ValidCardTags = InValidCardTags;

	// 카드 선택 명령 텍스트를 설정
	const FString CommandString = FString::Printf(TEXT("Select %d Cards"), CardCount);
	CommandText->SetText(FText::FromString(CommandString));

	// 카드 선택 슬롯을 생성하고 UI에 추가
	for (int i = 1; i < CardCount; ++i)
	{
		// 고유한 이름을 가진 새로운 카드 선택 슬롯을 생성
		FName UniqueName = MakeUniqueObjectName(this, UUW_CardListCard::StaticClass());
		UUW_CardListCard* NewCardListCard = CreateWidget<UUW_CardListCard>(GetWorld(), WBP_CardListCardClass, UniqueName);

		// 카드 선택 박스에 슬롯 추가
		CardSelectionBox->AddChild(NewCardListCard);

		// 카드 슬롯을 가시화
		NewCardListCard->SetVisibility(ESlateVisibility::Visible);

		// 카드 클릭 시 해당 카드를 반환하는 이벤트 연결
		NewCardListCard->OnCardClicked.AddDynamic(this, &UUW_HandSelect::ReturnCardIfValid);
	}
}

bool UUW_HandSelect::AttemptAddCard(ACardBase* CardActor, UUW_Card* PairedCardWidgetIn)
{
	// 선택 박스 안의 모든 카드 선택 슬롯을 순회
	for (UWidget* CardSelectionWidget : CardSelectionBox->GetAllChildren())
	{
		// 슬롯이 UUW_CardListCard 타입인지 확인
		UUW_CardListCard* CardListCardWidget = Cast<UUW_CardListCard>(CardSelectionWidget);
		if (!CardListCardWidget) continue;

		// 슬롯에 이미 카드가 있다면 건너뜀
		if (IsValid(CardListCardWidget->CardActor))continue;

		// 카드가 유효한 태그를 가지거나 유효 태그가 없는 경우 처리
		if (CardActor->GetGameplayTags().HasAnyExact(ValidCardTags) || ValidCardTags.IsEmpty())
		{
			// 카드 위젯을 업데이트하고 연결된 위젯을 설정
			IInterface_CardWidget::Execute_UpdateCardWidget(CardListCardWidget, CardActor);
			CardListCardWidget->PairedWidget = PairedCardWidgetIn;

			// 슬롯을 가시화하고 애니메이션 재생
			CardListCardWidget->SetVisibility(ESlateVisibility::Visible);
			IInterface_CardWidget::Execute_AnimateCardAppear(CardListCardWidget->CardVisual, EUMGSequencePlayMode::Forward);
			CardListCardWidget->PlayAnimationReverse(CardListCardWidget->FadeOut);

			// 모든 슬롯이 채워졌는지 확인하고, 확인 버튼 활성화
			EnableConfirmIfFull();
			return true;
		}
	}

	return false;
}

bool UUW_HandSelect::EnableConfirmIfFull()
{
	// 현재 선택된 카드 목록을 가져옴
	TArray<ACardBase*> SelectedCards;

	// 슬롯이 모두 채워졌는지 여부 확인
	bool bIsFull = GetContainedCards(SelectedCards);

	// 확인 버튼을 활성화 또는 비활성화
	ConfirmButton->SetIsEnabled(bIsFull);

	// 슬롯이 모두 찼는지 여부 반환
	return bIsFull;
}

bool UUW_HandSelect::GetContainedCards(TArray<ACardBase*>& ContainedCards)
{
	// 선택된 카드를 저장할 로컬 배열
	TArray<ACardBase*> SelectedCards;

	// 카드 선택 박스 내의 모든 위젯을 가져옴
	TArray<UWidget*> CardSelectionWidgets = CardSelectionBox->GetAllChildren();

	// 각 위젯을 검사하여, 카드 슬롯 위젯일 경우 선택된 카드를 수집
	for (UWidget* Widget : CardSelectionWidgets)
	{
		if (UUW_CardListCard* CardListCard = Cast<UUW_CardListCard>(Widget))
		{
			// 카드 슬롯에 연결된 카드 액터를 로컬 배열에 추가
			SelectedCards.Add(CardListCard->CardActor);
		}
	}

	// 수집된 카드를 참조로 전달받은 배열에 복사
	ContainedCards = SelectedCards;

	// 모든 슬롯이 채워졌는지 여부를 반환
	return CardSelectionWidgets.Num() == SelectedCards.Num();
}

void UUW_HandSelect::ConfirmButtonOnClicked()
{
	// 선택된 카드 목록을 가져옴
	TArray<ACardBase*> SelectedCards;
	if (!GetContainedCards(SelectedCards)) return;

	// 선택된 카드들을 델리게이트를 통해 브로드캐스트
	OnConfirmCards.Broadcast(SelectedCards);

	// 모든 카드 선택 슬롯의 가시성을 비활성화
	for (UWidget* ChildWidget : CardSelectionBox->GetAllChildren())
	{
		// 카드 슬롯 위젯을 캐스팅
		UUW_CardListCard* CardListCard = Cast<UUW_CardListCard>(ChildWidget);
		if (!CardListCard) continue;

		// 연결된 카드 위젯의 가시성을 숨김
		CardListCard->PairedWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UUW_HandSelect::ReturnCardIfValid(UUW_CardListCard* CardListCard, ACardBase* CardActor)
{
	// 카드가 유효한지 확인
	if (IsValid(CardActor))
	{
		// 선택 슬롯에서 카드를 제거하는 애니메이션 실행
		PlayAnimation(CardListCard->FadeOut);

		// 카드 슬롯을 비워줌
		CardListCard->CardActor = nullptr;

		// 카드 반환 이벤트를 델리게이트를 통해 브로드캐스트
		if (OnReturnCard.IsBound())
		{
			OnReturnCard.Broadcast(CardListCard->PairedWidget);
		}

		// 연결된 카드 위젯을 비워줌
		CardListCard->PairedWidget = nullptr;

		// 확인 버튼을 활성화할지 여부를 다시 확인
		EnableConfirmIfFull();
	}
}