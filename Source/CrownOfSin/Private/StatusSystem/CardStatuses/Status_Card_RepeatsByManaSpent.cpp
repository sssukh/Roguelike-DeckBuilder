#include "StatusSystem/CardStatuses/Status_Card_RepeatsByManaSpent.h"

#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "CardSystem/CardUseRules/UseRule_StatCost.h"
#include "Core/DispatcherHubComponent.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "StatusSystem/Status_Mana.h"
#include "Utilities/CosGameplayTags.h"


// Sets default values for this component's properties
UStatus_Card_RepeatsByManaSpent::UStatus_Card_RepeatsByManaSpent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	Priority = 1.0f;
}


void UStatus_Card_RepeatsByManaSpent::BeginPlay()
{
	Super::BeginPlay();

	if (ACardBase* CastingCard = Cast<ACardBase>(GetOwner()))
	{
		OwningCard = CastingCard;

		UDispatcherHubComponent* DispatcherHub;
		if (GetOwnersDispatcherHub(DispatcherHub))
		{
			FGameplayTagContainer EventTags;
			EventTags.AddTag(CosGameTags::Event_Card_EnterHand);
			EventTags.AddTag(CosGameTags::Event_Card_ExitHand);
			EventTags.AddTag(CosGameTags::Event_Card_ModifyInHand);
			EventTags.AddTag(CosGameTags::Event_Card_PrePlay);
			DispatcherHub->BindMultipleEventsToHub(this, EventTags);
		}
	}
}

void UStatus_Card_RepeatsByManaSpent::UpdateFromMana(int32 NewMana)
{
	OwningCard->SetUseRuleCost(UUseRule_StatCost::StaticClass(), UStatus_Mana::StaticClass(), FMath::Max(1, NewMana), ECardDataType::Hand, true);

	int32 NewRepetitions = FMath::Max(NewMana, 1) - 1;
	OwningCard->SetCardRepetitions(ECardDataType::Hand, NewRepetitions);

	if (OwningCard->OnThisCardModified.IsBound())
		OwningCard->OnThisCardModified.Broadcast();
}

void UStatus_Card_RepeatsByManaSpent::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	if (EventTag == CosGameTags::Event_Card_EnterHand)
	{
		CardPlayer->DispatcherHubLocalComponent->BindEventToHub(this, CosGameTags::Event_PostModifyStatus);
		UpdateFromMana(CardPlayer->Status_Mana->StatusValue);
	}
	else if (EventTag == CosGameTags::Event_Card_ExitHand || EventTag == CosGameTags::Event_Card_PrePlay)
	{
		CardPlayer->DispatcherHubLocalComponent->UnbindEventFromHub(this, CosGameTags::Event_PostModifyStatus);
	}

	else if (EventTag == CosGameTags::Event_PostModifyStatus)
	{
		if (UStatus_Mana* StatusMana = Cast<UStatus_Mana>(CallingObject))
		{
			UpdateFromMana(StatusMana->StatusValue);
		}
	}
	else if (EventTag == CosGameTags::Event_Card_ModifyInHand)
	{
		UStatus_Mana* StatusMana = CardPlayer->FindComponentByClass<UStatus_Mana>();
		UpdateFromMana(StatusMana->StatusValue);
	}
}
