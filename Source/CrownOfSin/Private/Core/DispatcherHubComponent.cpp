#include "Core/DispatcherHubComponent.h"

#include "Interfaces/Interface_CardGameInstance.h"
#include "Interfaces/Interface_EventHolder.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UDispatcherHubComponent::UDispatcherHubComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}


// Called when the game starts
void UDispatcherHubComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UDispatcherHubComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDispatcherHubComponent::BindEventToHub(UObject* EventHolder, FGameplayTag EventTag)
{
	if (!IsValid(EventHolder))
		return;

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (!GameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		COS_LOG_SCREEN(TEXT("Game Instance 가 UInterface_CardInstance를 상속받지 못했습니다."));
		return;
	}

	if (IInterface_CardGameInstance::Execute_IsDebugMode(GameInstance))
	{
		UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_Debug_DispathcerHubUpdate, this, nullptr);
	}
	
	FObjectArray& EventArray = EventLists.FindOrAdd(EventTag);

	if (EventArray.Objects.Contains(EventHolder))
		return;
	
	// if (!IsValid(EventHolder))
	// 	return;
	//
	// UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	// if (!GameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	// {
	// 	COS_LOG_SCREEN(TEXT("Game Instacne 가 UInterface_CardInstance를 상속받지 못했습니다."));
	// 	return;
	// }
	//
	// if (IInterface_CardGameInstance::Execute_IsDebugMode(GameInstance))
	// {
	// 	UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_Debug_DispathcerHubUpdate, this, nullptr);
	// }
	//
	// if (!EventLists.Contains(EventTag))
	// {
	// 	FObjectArray NewObjectArray;
	// 	NewObjectArray.Objects = {EventHolder};
	// 	EventLists.Add(EventTag, NewObjectArray);
	// 	return;
	// }
	//
	// if (EventLists[EventTag].Objects.Contains(EventHolder))
	// {
	// 	return;
	// }
	//
	// if (!EventHolder->GetClass()->ImplementsInterface(UInterface_EventHolder::StaticClass()))
	// {
	// 	return;
	// }
	//
	// float HolderPriority = IInterface_EventHolder::Execute_GetPriority(EventHolder);
	//
	// TArray<UObject*> LocalInvalidObjects;
	// TArray<UObject*> LocalPriorityList;
	// int32 Index = 0;
	// for (UObject* EventObject : EventLists[EventTag].Objects)
	// {
	// 	if (IsValid(EventObject))
	// 	{
	// 		if (!EventObject->GetClass()->ImplementsInterface(UInterface_EventHolder::StaticClass()))
	// 			continue;
	// 		float ObjectPriority = IInterface_EventHolder::Execute_GetPriority(EventObject);
	//
	// 		if (ObjectPriority >= HolderPriority)
	// 		{
	// 			LocalPriorityList = EventLists[EventTag].Objects;
	// 			LocalPriorityList.Insert(EventHolder, Index + 1);
	//
	//
	// 			//진입점1
	// 			EventLists.Add(EventTag, FObjectArray(LocalPriorityList));
	//
	// 			if (LocalInvalidObjects.Num() <= 0)
	// 			{
	// 				return;
	// 			}
	//
	// 			FObjectArray* LocalObjects = EventLists.Find(EventTag);
	// 			if (!LocalObjects)
	// 			{
	// 				return;
	// 			}
	//
	// 			for (UObject* LocalInvalidObject : LocalInvalidObjects)
	// 			{
	// 				LocalObjects->Objects.Remove(LocalInvalidObject);
	// 			}
	//
	//
	// 			EventLists.Add(EventTag, FObjectArray(*LocalObjects));
	// 			return;
	// 		}
	// 	}
	// 	else
	// 	{
	// 		LocalInvalidObjects.AddUnique(EventObject);
	// 	}
	//
	// 	Index++;
	// }
	//
	//
	// LocalPriorityList = EventLists[EventTag].Objects;
	// LocalPriorityList.Insert(EventHolder, 0);
	//
	// //진입점1
	// EventLists.Add(EventTag, FObjectArray(LocalPriorityList));
	//
	// if (LocalInvalidObjects.Num() <= 0)
	// {
	// 	return;
	// }
	//
	// FObjectArray* LocalObjects = EventLists.Find(EventTag);
	// if (!LocalObjects)
	// {
	// 	return;
	// }
	//
	// for (UObject* LocalInvalidObject : LocalInvalidObjects)
	// {
	// 	LocalObjects->Objects.Remove(LocalInvalidObject);
	// }
	//
	//
	// EventLists.Add(EventTag, FObjectArray(*LocalObjects));
}

bool UDispatcherHubComponent::IsValidEventHolder(UObject* EventHolder)
{
	return EventHolder->GetClass()->ImplementsInterface(UInterface_EventHolder::StaticClass());
}

void UDispatcherHubComponent::Interrupt_Implementation()
{
	bInterrupt = true;
}

void UDispatcherHubComponent::CancelInterruption_Implementation()
{
	bInterrupt = false;
}
