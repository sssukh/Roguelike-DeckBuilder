#include "StatusSystem/CardStatuses/Status_Card_PlayCardOnCardPlayerEvent.h"

#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "Core/DispatcherHubComponent.h"
#include "Core/DispatcherHubLocalComponent.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"


// Sets default values for this component's properties
UStatus_Card_PlayCardOnCardPlayerEvent::UStatus_Card_PlayCardOnCardPlayerEvent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	Priority = 1.0f;
}


void UStatus_Card_PlayCardOnCardPlayerEvent::BeginPlay()
{
	Super::BeginPlay();

	UDispatcherHubComponent* DispatcherHub;
	if (GetOwnersDispatcherHub(DispatcherHub))
	{
		FGameplayTagContainer EventTags;
		EventTags.AddTag(CosGameTags::Event_Card_EnterHand);
		EventTags.AddTag(CosGameTags::Event_Card_ExitHand);
		DispatcherHub->BindMultipleEventsToHub(this, EventTags);
	}
}

void UStatus_Card_PlayCardOnCardPlayerEvent::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (EventTag == CosGameTags::Event_Card_EnterHand)
	{
		ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
		CardPlayer->DispatcherHubLocalComponent->BindMultipleEventsToHub(this, GameplayTags);

		RunEventGate.Open();
	}
	else if (EventTag == CosGameTags::Event_Card_ExitHand)
	{
		ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
		CardPlayer->DispatcherHubLocalComponent->UnbindMultipleEventsFromHub(this, GameplayTags);
		RunEventGate.Close();
	}
	else
	{
		if (RunEventGate.IsOpen())
		{
			if (ACardBase* OwnerCard = Cast<ACardBase>(GetOwner()))
			{
				UFunctionLibrary_Singletons::QueueDelay(this, 0.4f);
				UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_Action_AutoPlay, GetOwner(), nullptr, 1.0f);
				OwnerCard->AttemptUseCardUnTargeted(true, true, true);
				UFunctionLibrary_Singletons::QueueDelay(this, 1.0f);
			}
		}
	}
}
