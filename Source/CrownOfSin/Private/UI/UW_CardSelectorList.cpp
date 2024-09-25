#include "UI/UW_CardSelectorList.h"

#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Libraries/AssetPath.h"
#include "UI/UW_CardListCard.h"

UUW_CardSelectorList::UUW_CardSelectorList(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), CardPanel(nullptr), ListOverlay(nullptr), PileName(nullptr),
                                                                                          VisibilityButton(nullptr),
                                                                                          VisibilityButtonText(nullptr),
                                                                                          Attention(nullptr),
                                                                                          bDebugMode(false)
{
	static ConstructorHelpers::FClassFinder<UUW_CardListCard> WBP_CardListCard_C(*AssetPath::Blueprint::WBP_CardListCard_C);

	if (WBP_CardListCard_C.Succeeded())
	{
		CardListClass = WBP_CardListCard_C.Class;
	}
}

void UUW_CardSelectorList::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (!bDebugMode)
		return;

	for (int i = 0; i < 20; ++i)
	{
		UUserWidget* CardListCard = CreateWidget(GetWorld(), CardListClass);

		CardPanel->AddChildToUniformGrid(CardListCard, i / RowLength, i % RowLength);
	}
}

void UUW_CardSelectorList::NativeConstruct()
{
	Super::NativeConstruct();
	VisibilityButton->OnClicked.AddDynamic(this, &UUW_CardSelectorList::OnClicked_VisibilityButton);
}

void UUW_CardSelectorList::UpdateCardList(const TArray<ACardBase*>& Cards, const FText& Title)
{
	PileName->SetText(Title);

	CardPanel->ClearChildren();

	for (int index = 0; index < Cards.Num(); index++)
	{
		UUW_CardListCard* NewCardList = Cast<UUW_CardListCard>(CreateWidget(GetWorld(), CardListClass));
		NewCardList->CardActor = Cards[index];

		CardPanel->AddChildToUniformGrid(NewCardList, index / RowLength, index % RowLength);
		NewCardList->OnCardClicked.AddDynamic(this, &UUW_CardSelectorList::ReturnCardWhenClicked);
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UUW_CardSelectorList::ReturnCardWhenClicked(UUW_CardListCard* CardListCard, ACardBase* CardActor)
{
	if (OnReturnSelectedCard.IsBound())
		OnReturnSelectedCard.Broadcast(CardActor);

	SetVisibility(ESlateVisibility::Collapsed);
}

void UUW_CardSelectorList::OnClicked_VisibilityButton()
{
	ESlateVisibility NewVisibility = ESlateVisibility::Hidden;
	switch (ListOverlay->GetVisibility())
	{
	case ESlateVisibility::Visible:
		NewVisibility = ESlateVisibility::Hidden;
		break;
	case ESlateVisibility::Collapsed:
		NewVisibility = ESlateVisibility::Visible;
		break;
	case ESlateVisibility::Hidden:
		NewVisibility = ESlateVisibility::Visible;
		break;
	case ESlateVisibility::HitTestInvisible:
		NewVisibility = ESlateVisibility::Hidden;
		break;
	case ESlateVisibility::SelfHitTestInvisible:
		NewVisibility = ESlateVisibility::Hidden;
		break;
	default: ;
	}

	ListOverlay->SetVisibility(NewVisibility);

	bool IsListHidden = ListOverlay->GetVisibility() == ESlateVisibility::Hidden;
	FText NewText = IsListHidden ? FText::FromString(TEXT("Show")) : FText::FromString(TEXT("Hide"));
	VisibilityButtonText->SetText(NewText);

	if (IsListHidden)
	{
		PlayAnimation(Attention, 0, 0, EUMGSequencePlayMode::PingPong);
	}
	else
	{
		StopAnimation(Attention);
	}
}