#include "Libraries/FunctionLibrary_Event.h"

#include "Core/CosEnumStruct.h"
#include "Core/DispatcherHubComponent.h"
#include "Core/GlobalDispatcherHub.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/CosLog.h"

void UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(UObject* EventHolder, FGameplayTag EventTag)
{
	AGlobalDispatcherHub* GlobalDispatcherHub = Cast<AGlobalDispatcherHub>(UGameplayStatics::GetActorOfClass(EventHolder, AGlobalDispatcherHub::StaticClass()));
	if (!GlobalDispatcherHub)
	{
		COS_SCREEN(TEXT("레벨에 GlobalDispatcherHub가 존재하지 않습니다"));
		return;
	}

	GlobalDispatcherHub->DispatcherHubComponent->BindEventToHub(EventHolder, EventTag);
}

void UFunctionLibrary_Event::UnBindEventFromGlobalDispatcherHub(UObject* EventHolder, FGameplayTag EventTag)
{
	AGlobalDispatcherHub* GlobalDispatcherHub = Cast<AGlobalDispatcherHub>(UGameplayStatics::GetActorOfClass(EventHolder, AGlobalDispatcherHub::StaticClass()));
	if (!GlobalDispatcherHub)
	{
		COS_SCREEN(TEXT("레벨에 GlobalDispatcherHub가 존재하지 않습니다"));
		return;
	}

	GlobalDispatcherHub->DispatcherHubComponent->UnbindEventFromHub(EventHolder, EventTag);
}

bool UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject)
{
	AGlobalDispatcherHub* GlobalDispatcherHub = Cast<AGlobalDispatcherHub>(UGameplayStatics::GetActorOfClass(CallingObject, AGlobalDispatcherHub::StaticClass()));
	if (!GlobalDispatcherHub)
	{
		COS_SCREEN(TEXT("레벨에 GlobalDispatcherHub가 존재하지 않습니다"));
		return false;
	}

	return GlobalDispatcherHub->DispatcherHubComponent->CallEvent(EventTag, CallingObject, CallSpecificObject, ECallGlobal::OnlyLocal);
}

bool UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, float EndDelay, UObject* Payload,
                                                             FGameplayTagContainer CallTags)
{
	AGlobalDispatcherHub* GlobalDispatcherHub = Cast<AGlobalDispatcherHub>(UGameplayStatics::GetActorOfClass(CallingObject, AGlobalDispatcherHub::StaticClass()));
	if (!GlobalDispatcherHub)
	{
		COS_SCREEN(TEXT("레벨에 GlobalDispatcherHub가 존재하지 않습니다"));
		return false;
	}

	GlobalDispatcherHub->DispatcherHubComponent->QueueEventWithPayloadAndCallTags(EventTag, CallingObject, CallSpecificObject, EndDelay, Payload, CallTags);
	return true;
}

bool UFunctionLibrary_Event::CallMultipleEventsInGlobalDispatcherHub(FGameplayTagContainer EventTags, UObject* CallingObject, UObject* CallSpecificObject)
{
	AGlobalDispatcherHub* GlobalDispatcherHub = Cast<AGlobalDispatcherHub>(UGameplayStatics::GetActorOfClass(CallingObject, AGlobalDispatcherHub::StaticClass()));
	if (!GlobalDispatcherHub)
	{
		COS_SCREEN(TEXT("레벨에 GlobalDispatcherHub가 존재하지 않습니다"));
		return false;
	}

	return GlobalDispatcherHub->DispatcherHubComponent->CallMultipleBoundEvents(EventTags.GetGameplayTagArray(), CallingObject, CallSpecificObject, ECallGlobal::CallAfter);
}

void UFunctionLibrary_Event::BindMultipleEventsToGlobalDispatcherHub(UObject* EventHolder, FGameplayTagContainer EventTags)
{
	AGlobalDispatcherHub* GlobalDispatcherHub = Cast<AGlobalDispatcherHub>(UGameplayStatics::GetActorOfClass(EventHolder, AGlobalDispatcherHub::StaticClass()));
	if (!GlobalDispatcherHub)
	{
		COS_SCREEN(TEXT("레벨에 GlobalDispatcherHub가 존재하지 않습니다"));
		return;
	}

	for (const FGameplayTag& EventTag : EventTags.GetGameplayTagArray())
	{
		GlobalDispatcherHub->DispatcherHubComponent->BindEventToHub(EventHolder, EventTag);
	}
}
