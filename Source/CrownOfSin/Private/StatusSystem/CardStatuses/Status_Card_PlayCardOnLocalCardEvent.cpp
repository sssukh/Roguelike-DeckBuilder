#include "StatusSystem/CardStatuses/Status_Card_PlayCardOnLocalCardEvent.h"

#include "CardSystem/CardBase.h"
#include "Core/DispatcherHubComponent.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"


UStatus_Card_PlayCardOnLocalCardEvent::UStatus_Card_PlayCardOnLocalCardEvent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	Priority = -1.0f;
}


// Called when the game starts
void UStatus_Card_PlayCardOnLocalCardEvent::BeginPlay()
{
	Super::BeginPlay();

	UDispatcherHubComponent* DispatcherHub;
	if (GetOwnersDispatcherHub(DispatcherHub))
	{
		DispatcherHub->BindMultipleEventsToHub(this, GameplayTags);
	}
}

void UStatus_Card_PlayCardOnLocalCardEvent::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (ACardBase* OwnerCard = Cast<ACardBase>(GetOwner()))
	{
		UFunctionLibrary_Singletons::QueueDelay(this, 0.4f);
		UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(CosGameTags::Event_Action_AutoPlay, GetOwner(), nullptr, 1.0f);

		if (OwnerCard->GetGameplayTags().HasTagExact(CosGameTags::Flag_KeepAfterAutoPlay))
		{
			OriginalPostUseEvent = OwnerCard->GetPostUseEvent(ECardDataType::Hand);
			OwnerCard->SetPostUseEvent(ECardDataType::Hand, CosGameTags::Event_Card_ReturnToHand);
			OwnerCard->AttemptUseCardUnTargeted(true, true, true);
			OwnerCard->SetPostUseEvent(ECardDataType::Hand, OriginalPostUseEvent);
			UFunctionLibrary_Singletons::QueueDelay(this, 1.0f);
		}
		else
		{
			OwnerCard->AttemptUseCardUnTargeted(true, true, true);
			UFunctionLibrary_Singletons::QueueDelay(this, 1.0f);
		}
	}
}
