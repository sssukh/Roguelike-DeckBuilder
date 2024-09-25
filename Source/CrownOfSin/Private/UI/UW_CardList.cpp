#include "UI/UW_CardList.h"

#include "CardSystem/Piles/PileComponent.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Libraries/AssetPath.h"
#include "UI/UW_CardListCard.h"
#include "Utilities/CosLog.h"

UUW_CardList::UUW_CardList(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), CardPanel(nullptr), PileName(nullptr), InsideButton(nullptr), OutsideButton(nullptr),
                                                                          PileReference(nullptr)
{
	static ConstructorHelpers::FClassFinder<UUW_CardListCard> WBP_CardListCard_C(*AssetPath::Blueprint::WBP_CardListCard_C);
	if (WBP_CardListCard_C.Succeeded())
	{
		WBP_CardListCardClass = WBP_CardListCard_C.Class;
	}
	else
	{
		COS_LOG_ERROR(TEXT("WBP_CardListCardClass를 로드하지 못했습니다."));
	}
}

void UUW_CardList::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!bDebugDisplay) return;

	for (int i = 0; i < 20; ++i)
	{
		UUW_CardListCard* NewCardListCard = CreateWidget<UUW_CardListCard>(GetWorld(), WBP_CardListCardClass);
		CardPanel->AddChildToUniformGrid(NewCardListCard, i / RowLength, i % RowLength);
	}
}

void UUW_CardList::NativeConstruct()
{
	Super::NativeConstruct();

	PileName->SetText(PileReference->FriendlyName);

	UpdateCardList();

	InsideButton->OnClicked.AddDynamic(this, &UUW_CardList::OnClicked_InsideButton);
	OutsideButton->OnClicked.AddDynamic(this, &UUW_CardList::OnClicked_OutsideButton);
}

void UUW_CardList::UpdateCardList()
{
	CardPanel->ClearChildren();

	int32 Index = 0;
	for (ACardBase* Card : PileReference->Cards)
	{
		UUW_CardListCard* NewCardListCard = CreateWidget<UUW_CardListCard>(GetWorld(), WBP_CardListCardClass);
		NewCardListCard->CardActor = Card;
		CardPanel->AddChildToUniformGrid(NewCardListCard, Index / RowLength, Index % RowLength);
		Index++;
	}
	SetFocus();
}

void UUW_CardList::OnClicked_InsideButton()
{
	if (OnOutSideClicked.IsBound())
		OnOutSideClicked.Broadcast();
}

void UUW_CardList::OnClicked_OutsideButton()
{
	if (OnOutSideClicked.IsBound())
		OnOutSideClicked.Broadcast();
}
