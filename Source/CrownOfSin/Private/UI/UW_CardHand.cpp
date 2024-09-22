#include "UI/UW_CardHand.h"


#include "Libraries/FunctionLibrary_Event.h"
#include "Utilities/CosGameplayTags.h"
#include "UI/UW_Anchor.h"
#include "UI/UW_HandSelect.h"

UUW_CardHand::UUW_CardHand(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), WBP_HandAnchor(nullptr), WBP_PlayAnchor(nullptr), WBP_ReshuffleAnchor(nullptr),
                                                                          bSelectionMode(false)
{
}

void UUW_CardHand::NativeConstruct()
{
	Super::NativeConstruct();

	TMap<FGameplayTag, UUserWidget*> NewAnchorWidgets;
	NewAnchorWidgets.Add(CosGameTags::Pile_Hand, WBP_HandAnchor);
	NewAnchorWidgets.Add(CosGameTags::Event_Action_AutoPlay, WBP_PlayAnchor);
	NewAnchorWidgets.Add(CosGameTags::Event_Action_GenerateCard, WBP_PlayAnchor);

	AddAnchorWidgets(NewAnchorWidgets);
	UpdatePeriodically();

	FGameplayTagContainer EventTags;
	EventTags.AddTag(CosGameTags::Event_Action_AutoPlay);
	EventTags.AddTag(CosGameTags::Event_Action_FlickCard);
	EventTags.AddTag(CosGameTags::Event_Action_GenerateCard);
	EventTags.AddTag(CosGameTags::Event_Action_ResourceChange);
	EventTags.AddTag(CosGameTags::Event_Action_UseCard);
	EventTags.AddTag(CosGameTags::Event_Action_Victory);
	EventTags.AddTag(CosGameTags::Event_CardSelectionMode);
	EventTags.AddTag(CosGameTags::Event_GameOver);
	EventTags.AddTag(CosGameTags::Event_Victory);
	for (const FGameplayTag& EventTag : EventTags.GetGameplayTagArray())
	{
		UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(this, EventTag);
	}
}

void UUW_CardHand::AddAnchorWidgets(const TMap<FGameplayTag, UUserWidget*>& InAnchorWidgets)
{
	for (const TTuple<FGameplayTag, UUserWidget*>& InAnchorWidget : InAnchorWidgets)
	{
		AnchorWidgets.Add(InAnchorWidget);
	}
}

void UUW_CardHand::UpdatePeriodically()
{
	UpdateCardTransforms();
	GetWorld()->GetTimerManager().SetTimer(UpdateTimerHandle, this, &UUW_CardHand::OnUpdateCardTransforms, UpdatePeriod, true);
}

void UUW_CardHand::UpdateCardTransforms()
{
	
}

void UUW_CardHand::EnableSelectionMode(int32 CardCount, const FGameplayTagContainer& ValidCardTags)
{
	WBP_HandSelect->InitializeEvent(CardCount, this, ValidCardTags);

	bSelectionMode = true;

	WBP_HandSelect->SetVisibility(ESlateVisibility::Visible);
}

void UUW_CardHand::OnUpdateCardTransforms()
{
	UpdateCardTransforms();
}
