#include "StatusSystem/CardStatuses/Status_Card_InterruptOnPlayerUse.h"

#include "Core/DispatcherHubComponent.h"
#include "Utilities/CosGameplayTags.h"


UStatus_Card_InterruptOnPlayerUse::UStatus_Card_InterruptOnPlayerUse()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	Priority = 1.0f;
}


void UStatus_Card_InterruptOnPlayerUse::BeginPlay()
{
	Super::BeginPlay();

	UDispatcherHubComponent* DispatcherHub;
	if (GetOwnersDispatcherHub(DispatcherHub))
	{
		DispatcherHub->BindEventToHub(this, CosGameTags::Event_Card_PrePlay);
	}
}

void UStatus_Card_InterruptOnPlayerUse::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (!CallTags.HasTagExact(CosGameTags::Flag_AutoPlayed))
	{
		if (GetOwner()->Implements<UInterface_Interrupt>())
			IInterface_Interrupt::Execute_Interrupt(GetOwner());
	}
}
