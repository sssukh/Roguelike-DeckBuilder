#include "UI/UW_CardHand.h"


#include "Utilities/CosGameplayTags.h"
#include "UI/UW_Anchor.h"
#include "UI/UW_HandSelect.h"

UUW_CardHand::UUW_CardHand(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), WBP_HandAnchor(nullptr), WBP_PlayAnchor(nullptr), WBP_ReshuffleAnchor(nullptr)
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
	GetWorld()->GetTimerManager().SetTimer(UpdateTimerHandle, this, &UUW_CardHand::UpdateCardTransforms, UpdatePeriod, true);
}

void UUW_CardHand::EnableSelectionMode(int32 CardCount, const FGameplayTagContainer& ValidCardTags)
{
	WBP_HandSelect->Initialize(CardCount,this, ValidCardTags);

	bSelectionMode = true;

	WBP_HandSelect->SetVisibility(ESlateVisibility::Visible);
}

void UUW_CardHand::UpdateCardTransforms()
{
	
}
