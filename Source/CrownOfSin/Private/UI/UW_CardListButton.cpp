#include "UI/UW_CardListButton.h"

#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileDeckComponent.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/KismetTextLibrary.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosLog.h"

#include "UI/UW_ToolTip.h"
#include "UI/UW_CardList.h"

UUW_CardListButton::UUW_CardListButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), TextOverlay(nullptr), Text(nullptr), AmountText(nullptr), DeckIconImage(nullptr),
                                                                                      DeckButtonBorder(nullptr),
                                                                                      CardListButton(nullptr),
                                                                                      Add(nullptr), bFlip(false),
                                                                                      PileReference(nullptr),
                                                                                      CardList(nullptr),
                                                                                      WidgetSwitcherReference(nullptr)
{
	ButtonText = FText::FromString(TEXT("N/A"));

	static ConstructorHelpers::FClassFinder<UUW_ToolTip> WBP_ToolTipClass_C(*AssetPath::Blueprint::WBP_ToolTip_C);
	if (WBP_ToolTipClass_C.Succeeded())
	{
		WBP_ToolTipClass = WBP_ToolTipClass_C.Class;
	}
	else
	{
		COS_LOG_ERROR(TEXT("WBP_ToolTipClass를 로드하지 못했습니다."));
	}

	static ConstructorHelpers::FClassFinder<UUW_CardList> WBP_CardList_C(*AssetPath::Blueprint::WBP_CardList_C);
	if (WBP_CardList_C.Succeeded())
	{
		WBP_CardListClass = WBP_CardList_C.Class;
	}
	else
	{
		COS_LOG_ERROR(TEXT("WBP_CardListClass를 로드하지 못했습니다."));
	}


	static ConstructorHelpers::FObjectFinder<UTexture2D> T_Dagger(*AssetPath::Texture::T_Dagger);
	if (T_Dagger.Succeeded())
	{
		DeckIcon = T_Dagger.Object;
	}
	else
	{
		COS_LOG_ERROR(TEXT("T_Dagger를 찾지 못했습니다"));
	}

	DeckTint = FLinearColor(1, 1, 1, 1);
	PileClass = UPileDeckComponent::StaticClass();
}

void UUW_CardListButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	Text->SetText(ButtonText);

	DeckIconImage->SetBrushFromTexture(DeckIcon);

	DeckButtonBorder->SetContentColorAndOpacity(DeckTint);

	if (bFlip)
	{
		CardListButton->SetRenderScale(FVector2D(-1.0f, 1.0f));
		UOverlaySlot* TextOverlaySlot = Cast<UOverlaySlot>(TextOverlay->Slot);
		TextOverlaySlot->SetPadding(FMargin(0, 0, DefaultIconLeftPadding, 5.0));
	}
	else
	{
		CardListButton->SetRenderScale(FVector2D(1.0f, 1.0f));
		UOverlaySlot* TextOverlaySlot = Cast<UOverlaySlot>(TextOverlay->Slot);
		TextOverlaySlot->SetPadding(FMargin(DefaultIconLeftPadding, 0, 0, 5.0));
	}
}

void UUW_CardListButton::NativeConstruct()
{
	Super::NativeConstruct();

	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	COS_IF_CHECK(CardPlayer, TEXT("CardPlayer를 찾지 못했습니다."));

	PileReference = Cast<UPileComponent>(CardPlayer->GetComponentByClass(PileClass));

	UUW_ToolTip* NewToolTip = CreateWidget<UUW_ToolTip>(GetWorld(), WBP_ToolTipClass);
	CardListButton->SetToolTip(NewToolTip);

	CardList = CreateWidget<UUW_CardList>(GetWorld(), WBP_CardListClass);
	CardList->PileReference = PileReference;

	CardList->OnOutSideClicked.AddDynamic(this, &UUW_CardListButton::HideOnOutsideClicked);

	if (IsValid(WidgetSwitcherReference))
	{
		WidgetSwitcherReference->AddChild(CardList);
	}

	CardListButton->OnClicked.AddDynamic(this, &UUW_CardListButton::OnClicked_CardListButton);
}

void UUW_CardListButton::UpdateAmountFromPileRef()
{
	UpdateAmountDirectly(PileReference->Cards.Num());
}

void UUW_CardListButton::UpdateAmountDirectly(int32 NewAmount)
{
	AmountText->SetText(UKismetTextLibrary::Conv_IntToText(NewAmount));
	PlayAnimation(Add);
}

void UUW_CardListButton::HideOnOutsideClicked()
{
	if (IsValid(WidgetSwitcherReference))
	{
		WidgetSwitcherReference->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUW_CardListButton::OnClicked_CardListButton()
{
	if (IsValid(WidgetSwitcherReference))
	{
		ESlateVisibility SelectSlateVisibility = ESlateVisibility::Visible;
		if (WidgetSwitcherReference->GetVisibility() == ESlateVisibility::Visible) SelectSlateVisibility = ESlateVisibility::Hidden;
		if (WidgetSwitcherReference->GetVisibility() == ESlateVisibility::Collapsed) SelectSlateVisibility = ESlateVisibility::Visible;
		if (WidgetSwitcherReference->GetVisibility() == ESlateVisibility::Hidden) SelectSlateVisibility = ESlateVisibility::Visible;
		if (WidgetSwitcherReference->GetVisibility() == ESlateVisibility::HitTestInvisible) SelectSlateVisibility = ESlateVisibility::Visible;
		if (WidgetSwitcherReference->GetVisibility() == ESlateVisibility::SelfHitTestInvisible) SelectSlateVisibility = ESlateVisibility::Visible;
		ESlateVisibility NewSlateVisibility = WidgetSwitcherReference->GetActiveWidget() == CardList ? SelectSlateVisibility : ESlateVisibility::Visible;

		WidgetSwitcherReference->SetVisibility(NewSlateVisibility);
		CardList->UpdateCardList();
		WidgetSwitcherReference->SetActiveWidget(CardList);
	}
}
