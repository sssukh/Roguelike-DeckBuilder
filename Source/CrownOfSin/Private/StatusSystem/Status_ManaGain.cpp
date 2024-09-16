#include "StatusSystem/Status_ManaGain.h"

#include "Core/DispatcherHubComponent.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "StatusSystem/Status_Mana.h"
#include "Utilities/CosGameplayTags.h"

UStatus_ManaGain::UStatus_ManaGain()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TextAlignment = EStatusTextAlignment::Center;
}

void UStatus_ManaGain::BeginPlay()
{
	Super::BeginPlay();

	UDispatcherHubComponent* DispatcherHub;
	if (GetOwnersDispatcherHub(DispatcherHub))
	{
		DispatcherHub->BindMultipleEventsToHub(this, FGameplayTagContainer(CosGameTags::Event_TurnStart));
	}
}

void UStatus_ManaGain::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	AActor* CallActor = Cast<AActor>(CallingObject);
	if (!CallActor) return;

	UStatus_Mana* StatusMana = CallActor->FindComponentByClass<UStatus_Mana>();
	if (!StatusMana) return;
	
	if (EventTag == CosGameTags::Event_TurnStart)
	{
		UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_ManaGain, this);
		
		if (bInterrupt)
		{
			bInterrupt = false;
			return;
		}

		StatusMana->SetStatusValue(StatusValue);
	}
}
