#include "Core/DispatcherHubLocalComponent.h"

#include "Core/GlobalDispatcherHub.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/CosLog.h"


UDispatcherHubLocalComponent::UDispatcherHubLocalComponent(): GlobalDispatcherHubComponentRef(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}

void UDispatcherHubLocalComponent::BeginPlay()
{
	Super::BeginPlay();

	AGlobalDispatcherHub* GlobalDispatcherHub = Cast<AGlobalDispatcherHub>(UGameplayStatics::GetActorOfClass(this, AGlobalDispatcherHub::StaticClass()));
	if (!GlobalDispatcherHub)
	{
		COS_LOG_SCREEN(TEXT("World에서 GlobalDispatcherHub 액터를 찾지 못했습니다. 월드에 스폰해주세요"));
		return;
	}

	GlobalDispatcherHubComponentRef = GlobalDispatcherHub->DispatcherHubComponent;
}

void UDispatcherHubLocalComponent::BindEventToHub(UObject* EventHolder, FGameplayTag EventTag)
{
	Super::BindEventToHub(EventHolder, EventTag);

	bEmpty = false;
}

void UDispatcherHubLocalComponent::UnbindEventFromHub(UObject* EventHolder, FGameplayTag EventTag)
{
	Super::UnbindEventFromHub(EventHolder, EventTag);

	bEmpty = EventLists.IsEmpty();
}

bool UDispatcherHubLocalComponent::CallEvent(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	return ResolveCallEvent(EventTag, CallingObject, CallSpecificObject, AlsoCallGlobal, GameInstance, BlankContainer);
}

bool UDispatcherHubLocalComponent::CallEventWithCallTags(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal, FGameplayTagContainer CallTags)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	return ResolveCallEvent(EventTag, CallingObject, CallSpecificObject, AlsoCallGlobal, GameInstance, CallTags);
}

bool UDispatcherHubLocalComponent::CallEventWithPayload(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal, UObject* PayLoad)
{
	return ResolveCallEvent(EventTag, CallingObject, CallSpecificObject, AlsoCallGlobal, PayLoad, BlankContainer);
}

bool UDispatcherHubLocalComponent::CallEventWithPayloadAndCallTags(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal, UObject* PayLoad,
                                                                   FGameplayTagContainer CallTags)
{
	return ResolveCallEvent(EventTag, CallingObject, CallSpecificObject, AlsoCallGlobal, PayLoad, CallTags);
}

bool UDispatcherHubLocalComponent::ResolveCallEvent(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal, UObject* PayLoad,
                                                    FGameplayTagContainer CallTags)
{
	if (AlsoCallGlobal == ECallGlobal::OnlyLocal)
	{
		if (bEmpty)
			return true;

		return Super::ResolveCallEvent(EventTag, CallingObject, CallSpecificObject, AlsoCallGlobal, PayLoad, CallTags);
	}

	if (AlsoCallGlobal == ECallGlobal::CallBefore)
	{
		GlobalDispatcherHubComponentRef->ResolveCallEvent(EventTag, CallingObject, CallSpecificObject, AlsoCallGlobal, PayLoad, CallTags);

		if (bEmpty)
			return true;

		return Super::ResolveCallEvent(EventTag, CallingObject, CallSpecificObject, AlsoCallGlobal, PayLoad, CallTags);
	}

	if (AlsoCallGlobal == ECallGlobal::CallAfter)
	{
		if (!bEmpty)
			Super::ResolveCallEvent(EventTag, CallingObject, CallSpecificObject, AlsoCallGlobal, PayLoad, CallTags);

		return GlobalDispatcherHubComponentRef->ResolveCallEvent(EventTag, CallingObject, CallSpecificObject, AlsoCallGlobal, PayLoad, CallTags);
	}

	return false;
}
