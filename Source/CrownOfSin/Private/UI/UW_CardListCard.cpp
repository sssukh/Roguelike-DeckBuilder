#include "UI/UW_CardListCard.h"
#include "CardSystem/CardBase.h"
#include "Components/SizeBox.h"
#include "Interfaces/Widget/Interface_CardWidget.h"
#include "Libraries/AssetPath.h"
#include "UI/UW_CardVisual.h"
#include "UI/UW_ToolTipList.h"
#include "Utilities/CosLog.h"

UUW_CardListCard::UUW_CardListCard(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), bBlockHoverAnim(false)
{
	static ConstructorHelpers::FClassFinder<UUW_CardVisual> WBP_CardVisual (*AssetPath::Blueprint::WBP_CardVisual);
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

	IInterface_Utility::Execute_Initialize(this, FGameplayTagContainer());
}

UUserWidget* UUW_CardListCard::SetCardVisualWidget(ACardBase* InCardActor)
{
	CardActor = InCardActor;

	// CardVisual 이미 있으면 제거
	if (IsValid(CardVisual))
	{
		CardVisual->RemoveFromParent();
	}

	// CardActor 유효하면 CardVisualClass 지정
	if (IsValid(CardActor))
	{
		CardVisualClass = CardActor->GetCardVisualWidget(ECardDataType::Deck);
	}

	// CardVisual 새로 생성 및 설정
	CardVisual = CreateWidget(GetWorld(), CardVisualClass);
	CardVisualBox->AddChild(CardVisual);
	CardVisual->SetVisibility(ESlateVisibility::HitTestInvisible);

	return CardVisual;
}

void UUW_CardListCard::Initialize_Implementation(const FGameplayTagContainer& Tags)
{
	if (!IsValid(CardActor)) return;

	if (CardVisual->GetClass()->ImplementsInterface(UInterface_CardWidget::StaticClass()))
	{
		IInterface_CardWidget::Execute_UpdateCardWidget(CardVisual, CardActor);
	}

	for (const FDataTableRowHandle& Tooltip : CardActor->GetCardTooltips(ECardDataType::Hand))
	{
		WBP_TooltipList->AddToolTipFromData(Tooltip, 0);
	}
}

void UUW_CardListCard::UpdateCardWidget_Implementation(ACardBase* InCardActor)
{
	UUserWidget* CardVisualOut = SetCardVisualWidget(InCardActor);
	IInterface_CardWidget::Execute_UpdateCardWidget(CardVisualOut, InCardActor);
}
