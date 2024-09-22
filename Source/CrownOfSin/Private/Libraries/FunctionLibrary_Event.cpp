#include "Libraries/FunctionLibrary_Event.h"

#include "Core/CosEnumStruct.h"
#include "Core/DispatcherHubComponent.h"
#include "Core/GlobalDispatcherHub.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/CosLog.h"

void UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(UObject* EventHolder, FGameplayTag EventTag)
{
	if (AGlobalDispatcherHub* GlobalDispatcherHub = Cast<AGlobalDispatcherHub>(UGameplayStatics::GetActorOfClass(EventHolder, AGlobalDispatcherHub::StaticClass())))
	{
		GlobalDispatcherHub->DispatcherHubComponent->BindEventToHub(EventHolder, EventTag);
		return;
	}

	COS_LOG_SCREEN_ERROR(TEXT("레벨에 GlobalDispatcherHub가 존재하지 않습니다"));
}

void UFunctionLibrary_Event::UnBindEventFromGlobalDispatcherHub(UObject* EventHolder, FGameplayTag EventTag)
{
	if (AGlobalDispatcherHub* GlobalDispatcherHub = Cast<AGlobalDispatcherHub>(UGameplayStatics::GetActorOfClass(EventHolder, AGlobalDispatcherHub::StaticClass())))
	{
		GlobalDispatcherHub->DispatcherHubComponent->UnbindEventFromHub(EventHolder, EventTag);
		return;
	}

	COS_LOG_SCREEN_ERROR(TEXT("레벨에 GlobalDispatcherHub가 존재하지 않습니다"));
}

bool UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject)
{
	if (AGlobalDispatcherHub* GlobalDispatcherHub = Cast<AGlobalDispatcherHub>(UGameplayStatics::GetActorOfClass(CallingObject, AGlobalDispatcherHub::StaticClass())))
	{
		return GlobalDispatcherHub->DispatcherHubComponent->CallEvent(EventTag, CallingObject, CallSpecificObject, ECallGlobal::OnlyLocal);
	}

	COS_LOG_SCREEN_ERROR(TEXT("레벨에 GlobalDispatcherHub가 존재하지 않습니다"));
	return false;
}

bool UFunctionLibrary_Event::QueueEventInGlobalDispatcherHub(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, float EndDelay, UObject* Payload,
                                                             FGameplayTagContainer CallTags)
{
	if (AGlobalDispatcherHub* GlobalDispatcherHub = Cast<AGlobalDispatcherHub>(UGameplayStatics::GetActorOfClass(CallingObject, AGlobalDispatcherHub::StaticClass())))
	{
		GlobalDispatcherHub->DispatcherHubComponent->QueueEventWithPayloadAndCallTags(EventTag, CallingObject, CallSpecificObject, EndDelay, Payload, CallTags);
		return true;
	}

	COS_LOG_SCREEN_ERROR(TEXT("레벨에 GlobalDispatcherHub가 존재하지 않습니다"));
	return false;
}

bool UFunctionLibrary_Event::CallEventWithCallTagsInGlobalDispatcherHub(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, FGameplayTagContainer CallTags)
{
	if (AGlobalDispatcherHub* GlobalDispatcherHub = Cast<AGlobalDispatcherHub>(UGameplayStatics::GetActorOfClass(CallingObject, AGlobalDispatcherHub::StaticClass())))
	{
		return GlobalDispatcherHub->DispatcherHubComponent->CallEventWithCallTags(EventTag, CallingObject, CallSpecificObject, ECallGlobal::OnlyLocal, CallTags);
	}

	COS_LOG_SCREEN_ERROR(TEXT("레벨에 GlobalDispatcherHub가 존재하지 않습니다"));
	return false;
}

bool UFunctionLibrary_Event::CallMultipleEventsInGlobalDispatcherHub(FGameplayTagContainer EventTags, UObject* CallingObject, UObject* CallSpecificObject)
{
	if (AGlobalDispatcherHub* GlobalDispatcherHub = Cast<AGlobalDispatcherHub>(UGameplayStatics::GetActorOfClass(CallingObject, AGlobalDispatcherHub::StaticClass())))
	{
		return GlobalDispatcherHub->DispatcherHubComponent->CallMultipleBoundEvents(EventTags.GetGameplayTagArray(), CallingObject, CallSpecificObject, ECallGlobal::CallAfter);
	}

	COS_LOG_SCREEN_ERROR(TEXT("레벨에 GlobalDispatcherHub가 존재하지 않습니다"));
	return false;
}

void UFunctionLibrary_Event::BindMultipleEventsToGlobalDispatcherHub(UObject* EventHolder, FGameplayTagContainer EventTags)
{
	if (AGlobalDispatcherHub* GlobalDispatcherHub = Cast<AGlobalDispatcherHub>(UGameplayStatics::GetActorOfClass(EventHolder, AGlobalDispatcherHub::StaticClass())))
	{
		for (const FGameplayTag& EventTag : EventTags.GetGameplayTagArray())
		{
			GlobalDispatcherHub->DispatcherHubComponent->BindEventToHub(EventHolder, EventTag);
		}
	}

	COS_LOG_SCREEN_ERROR(TEXT("레벨에 GlobalDispatcherHub가 존재하지 않습니다"));
}
