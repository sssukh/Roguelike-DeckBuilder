#include "UI/UW_CardListCard.h"
#include "CardSystem/CardBase.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Interfaces/Widget/Interface_CardWidget.h"
#include "Libraries/AssetPath.h"
#include "UI/UW_CardVisual.h"
#include "UI/UW_ToolTipList.h"
#include "Utilities/CosLog.h"

UUW_CardListCard::UUW_CardListCard(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), CardVisualBox(nullptr), HoverButton(nullptr), WBP_TooltipList(nullptr), Hover(nullptr),
                                                                                  Select(nullptr),
                                                                                  FadeOut(nullptr),
                                                                                  bBlockHoverAnim(false),
                                                                                  CardActor(nullptr),
                                                                                  PairedWidget(nullptr),
                                                                                  CardVisual(nullptr)
{
	static ConstructorHelpers::FClassFinder<UUW_CardVisual> WBP_CardVisual (*AssetPath::Blueprint::WBP_CardVisual_C);
	if(WBP_CardVisual.Succeeded())
	{
		CardVisualClass = WBP_CardVisual.Class;
	}
	else
	{
		COS_SCREEN(TEXT("WBP_CardVisual 을 로드할 수 없습니다."));
	}
}

void UUW_CardListCard::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetCardVisualWidget(CardActor);
}

void UUW_CardListCard::NativeConstruct()
{
	Super::NativeConstruct();

	Execute_Initialize(this, FGameplayTagContainer());

	HoverButton->OnHovered.AddDynamic(this, &UUW_CardListCard::OnHovered_HoverButton);
	HoverButton->OnClicked.AddDynamic(this, &UUW_CardListCard::OnClicked_HoverButton);
	HoverButton->OnUnhovered.AddDynamic(this, &UUW_CardListCard::OnUnhovered_HoverButton);
}

UUserWidget* UUW_CardListCard::SetCardVisualWidget(ACardBase* InCardActor)
{
	// 카드 시각적 위젯 클래스가 설정되지 않았을 경우 오류 처리
	COS_IF_CHECK(CardVisualClass, TEXT("UUW_CardListCard에서 CardVisualClass를 설정해주세요"), nullptr);

	// 입력받은 카드 액터를 멤버 변수에 저장
	CardActor = InCardActor;

	// 기존의 카드 시각적 요소가 있으면 제거
	if (IsValid(CardVisual))
	{
		CardVisual->RemoveFromParent();
	}

	// 카드 액터가 유효하면 새로운 카드 시각적 위젯 클래스를 설정
	if (IsValid(CardActor))
	{
		CardVisualClass = CardActor->GetCardVisualWidget(ECardDataType::Deck);
	}

	// 새 카드 시각적 요소 위젯을 생성하고 카드 시각적 박스에 추가
	CardVisual = CreateWidget(GetWorld(), CardVisualClass);
	CardVisualBox->AddChild(CardVisual);

	// 카드 시각적 요소를 가시화
	CardVisual->SetVisibility(ESlateVisibility::HitTestInvisible);

	// 새롭게 설정된 카드 시각적 요소 위젯 반환
	return CardVisual;
}

void UUW_CardListCard::OnHovered_HoverButton()
{
	if (!bBlockHoverAnim)
	{
		PlayAnimation(Hover);
	}
}

void UUW_CardListCard::OnClicked_HoverButton()
{
	if (OnCardClicked.IsBound())
		OnCardClicked.Broadcast(this, CardActor);
}

void UUW_CardListCard::OnUnhovered_HoverButton()
{
	if (!bBlockHoverAnim)
	{
		PlayAnimation(Hover, 0, 1, EUMGSequencePlayMode::Reverse);
	}
}

void UUW_CardListCard::Initialize_Implementation(const FGameplayTagContainer& Tags)
{
	if (!IsValid(CardActor)) return;

	if (CardVisual->GetClass()->ImplementsInterface(UInterface_CardWidget::StaticClass()))
	{
		Execute_UpdateCardWidget(CardVisual, CardActor);
	}

	for (const FDataTableRowHandle& Tooltip : CardActor->GetCardTooltips(ECardDataType::Hand))
	{
		WBP_TooltipList->AddToolTipFromData(Tooltip, 0);
	}
}

void UUW_CardListCard::UpdateCardWidget_Implementation(ACardBase* InCardActor)
{
	UUserWidget* NewCardVisual = SetCardVisualWidget(InCardActor);
	IInterface_CardWidget::Execute_UpdateCardWidget(NewCardVisual, InCardActor);
}