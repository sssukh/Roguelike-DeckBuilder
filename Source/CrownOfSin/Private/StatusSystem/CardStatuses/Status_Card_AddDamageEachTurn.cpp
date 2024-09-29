#include "StatusSystem/CardStatuses/Status_Card_AddDamageEachTurn.h"

#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"


UStatus_Card_AddDamageEachTurn::UStatus_Card_AddDamageEachTurn()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Priority = 1.0f;
}

void UStatus_Card_AddDamageEachTurn::BeginPlay()
{
	Super::BeginPlay();

	if (ACardBase* CastingCard = Cast<ACardBase>(GetOwner()))
	{
		OwningCard = CastingCard;
		FGameplayTagContainer EventTags;
		EventTags.AddTag(CosGameTags::Event_Card_EnterHand);
		EventTags.AddTag(CosGameTags::Event_Card_ExitHand);
		EventTags.AddTag(CosGameTags::Event_Card_ModifyInHand);
		OwningCard->DispatcherHubLocal->BindMultipleEventsToHub(this, EventTags);

		ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
		CardPlayer->DispatcherHubLocalComponent->BindEventToHub(this, CosGameTags::Event_TurnEnd);
	}
}

void UStatus_Card_AddDamageEachTurn::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (EventTag == CosGameTags::Event_TurnEnd)
	{
		if (RunEventGate.IsOpen())
		{
			CurrentModification = CurrentModification + StatusValue;
			OwningCard->ModifyCardEffectValues(StatusValue, ECardDataType::Hand, FGameplayTagContainer(), FGameplayTagContainer(CosGameTags::Effect_Attack));
		}
	}
	else if (EventTag == CosGameTags::Event_Card_EnterHand)
	{
		RunEventGate.Open();
		OwningCard->ModifyCardEffectValues(CurrentModification, ECardDataType::Hand, FGameplayTagContainer(), FGameplayTagContainer(CosGameTags::Effect_Attack));
	}
	else if (EventTag == CosGameTags::Event_Card_ExitHand)
	{
		RunEventGate.Close();
	}
	else if (EventTag == CosGameTags::Event_Card_ModifyInHand)
	{
		OwningCard->ModifyCardEffectValues(CurrentModification, ECardDataType::Hand, FGameplayTagContainer(), FGameplayTagContainer(CosGameTags::Effect_Attack));
	}
}
