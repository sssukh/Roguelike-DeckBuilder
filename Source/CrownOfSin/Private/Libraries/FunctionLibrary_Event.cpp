﻿#include "Libraries/FunctionLibrary_Event.h"

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
		COS_LOG_SCREEN(TEXT("레벨에 GlobalDispatcherHub가 존재하지 않습니다"));
		return;
	}

	GlobalDispatcherHub->DispatcherHubComponent->BindEventToHub(EventHolder, EventTag);
}

bool UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject)
{
	AGlobalDispatcherHub* GlobalDispatcherHub = Cast<AGlobalDispatcherHub>(UGameplayStatics::GetActorOfClass(CallingObject, AGlobalDispatcherHub::StaticClass()));
	if (!GlobalDispatcherHub)
	{
		COS_LOG_SCREEN(TEXT("레벨에 GlobalDispatcherHub가 존재하지 않습니다"));
		return false;
	}

	return GlobalDispatcherHub->DispatcherHubComponent->CallEvent(EventTag,CallingObject,CallSpecificObject,ECallGlobal::OnlyLocal);
}
