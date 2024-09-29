#include "StatusSystem/CardStatuses/Status_Card_ManaModifier.h"

#include "CardSystem/CardBase.h"
#include "CardSystem/CardUseRules/UseRule_StatCost.h"
#include "StatusSystem/Status_Mana.h"
#include "Utilities/CosGameplayTags.h"


UStatus_Card_ManaModifier::UStatus_Card_ManaModifier()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UStatus_Card_ManaModifier::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (ACardBase* CastingCard = Cast<ACardBase>(GetOwner()))
	{
		OwningCard = CastingCard;
		FGameplayTagContainer EventTags;
		EventTags.AddTag(CosGameTags::Event_Card_ExitHand);
		EventTags.AddTag(CosGameTags::Event_Card_ModifyInHand);
		OwningCard->BindLocalCardEvents(this, EventTags);

		ModifyCost();
	}
}

void UStatus_Card_ManaModifier::ModifyCost()
{
	int32 StatusCost, UseRuleIndex;
	OwningCard->GetStatusCostValueFromUseRules(ECardDataType::Hand, UStatus_Mana::StaticClass(), StatusCost, UseRuleIndex);

	int32 NewValue = FMath::Max(StatusCost + StatusValue, 0);
	if (OwningCard->SetUseRuleCost(UUseRule_StatCost::StaticClass(), UStatus_Mana::StaticClass(), NewValue, ECardDataType::Hand, false))
	{
		if (OwningCard->OnThisCardModified.IsBound())
			OwningCard->OnThisCardModified.Broadcast();
	}
}

void UStatus_Card_ManaModifier::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (OwningCard == CallingObject)
	{
		if (EventTag == CosGameTags::Event_Card_ModifyInHand)
		{
			ModifyCost();
		}
		else if (EventTag == CosGameTags::Event_Card_ExitHand)
		{
			DestroyComponent();
		}
	}
}
