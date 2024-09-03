#include "Core/DispatcherHubComponent.h"


#include "CardSystem/CardActions/Action_DispatcherEvent.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Interfaces/Interface_EventHolder.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


UDispatcherHubComponent::UDispatcherHubComponent(): bInterrupt(false), bGlobal(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}

void UDispatcherHubComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDispatcherHubComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDispatcherHubComponent::BindEventToHub(UObject* EventHolder, FGameplayTag EventTag)
{
	if (!IsValid(EventHolder))
	{
		return;
	}

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (!IsValidGameInstance(GameInstance))
	{
		COS_LOG_SCREEN(TEXT("Game Instance가 UInterface_CardInstance를 상속받지 못했습니다."));
		return;
	}

	// 디버그 모드가 활성화되어 있는지 확인합니다.
	if (IInterface_CardGameInstance::Execute_IsDebugMode(GameInstance))
	{
		UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_Debug_DispatcherHubUpdate, this, nullptr);
	}

	// EventTag가 존재하지 않으면 새로 생성하고, EventHolder를 추가합니다.
	if (!EventLists.Contains(EventTag))
	{
		AddNewEventHolder(EventTag, EventHolder); // 새로운 EventTag와 EventHolder를 추가합니다.
		return;
	}

	// EventHolder가 이미 등록되어 있는지 확인합니다.
	if (EventLists[EventTag].Objects.Contains(EventHolder))
	{
		return; // 이미 등록된 경우 함수를 종료합니다.
	}

	// EventHolder가 UInterface_EventHolder 인터페이스를 구현하는지 확인합니다.
	if (!ImplementsEventHolderInterface(EventHolder))
	{
		return; // 구현하지 않으면 함수를 종료합니다.
	}

	// 우선순위에 따라 EventHolder를 삽입합니다.
	InsertEventHolderWithPriority(EventTag, EventHolder);
}


void UDispatcherHubComponent::BindMultipleEventsToHub(UObject* EventHolder, FGameplayTagContainer EventTags)
{
	TArray<FGameplayTag> EventTagList;
	EventTags.GetGameplayTagArray(EventTagList);

	for (FGameplayTag EventTag : EventTagList)
	{
		BindEventToHub(EventHolder, EventTag);
	}
}

void UDispatcherHubComponent::UnbindEventFromHub(UObject* EventHolder, FGameplayTag EventTag)
{
	// EventLists에 EventTag가 존재하는지 확인합니다.
	if (!EventLists.Contains(EventTag))
	{
		return; // EventTag가 없으면 함수를 종료합니다.
	}

	// EventTag에 연결된 이벤트 홀더 목록을 가져옵니다.
	TArray<UObject*>& EventHoldersArray = EventLists[EventTag].Objects;

	// EventHolder를 목록에서 제거하고, 제거된 수를 기록합니다.
	int32 RemovedCount = EventHoldersArray.Remove(EventHolder);

	// 제거된 객체가 없으면 함수를 종료합니다.
	if (RemovedCount <= 0)
	{
		return;
	}

	// 업데이트된 이벤트 홀더 배열을 EventLists에 다시 추가합니다.
	EventLists.Add(EventTag, FObjectArray(EventHoldersArray));

	// 디버그 모드를 위한 이벤트 호출을 트리거합니다.
	UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_Debug_DispatcherHubUpdate, this, nullptr);
}

void UDispatcherHubComponent::UnbindMultipleEventsFromHub(UObject* EventHolder, FGameplayTagContainer EventTags)
{
	TArray<FGameplayTag> EventTagList;
	EventTags.GetGameplayTagArray(EventTagList);

	for (FGameplayTag EventTag : EventTagList)
	{
		UnbindEventFromHub(EventHolder, EventTag);
	}
}

bool UDispatcherHubComponent::CallMultipleBoundEvents(const TArray<FGameplayTag>& EventTagsGameplayTags, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal)
{
	for (FGameplayTag EventTagsGameplayTag : EventTagsGameplayTags)
	{
		ResolveCallEvent(EventTagsGameplayTag, CallingObject, CallSpecificObject, AlsoCallGlobal);
	}
	return true;
}

bool UDispatcherHubComponent::CallEvent(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal)
{
	return ResolveCallEvent(EventTag, CallingObject, CallSpecificObject, AlsoCallGlobal, nullptr, FGameplayTagContainer());
}

void UDispatcherHubComponent::QueueEvent(FGameplayTag Event, UObject* CallingObject, UObject* CallSpecificObject, float EndDelay, UObject* PayLoad, FGameplayTagContainer CallTags)
{
	if (!Action_DispatcherEventClass)
	{
		COS_LOG_SCREEN(TEXT("Action_DispatcherEventClass 클래스를 설정해주세요."));
		return;
	}
	
	FTransform SpawnTransform = FTransform::Identity;

	if (AAction_DispatcherEvent* NewDispatcherEvent = GetWorld()->SpawnActorDeferred<AAction_DispatcherEvent>(Action_DispatcherEventClass, SpawnTransform,
	                                                                                                          nullptr, nullptr,
	                                                                                                          ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
	{
		NewDispatcherEvent->Event = Event;
		NewDispatcherEvent->CallingObject = CallingObject;
		NewDispatcherEvent->DispatcherHub = this;
		NewDispatcherEvent->AlsoCallGlobal = ECallGlobal::CallAfter;
		NewDispatcherEvent->PayLoad = PayLoad;
		NewDispatcherEvent->CallTags = CallTags;
		NewDispatcherEvent->EndDelay = EndDelay;
		NewDispatcherEvent->FinishSpawning(SpawnTransform);
	}
		
}

void UDispatcherHubComponent::QueueEventWithPayloadAndCallTags(FGameplayTag Event, UObject* CallingObject, UObject* CallSpecificObject, float EndDelay, UObject* PayLoad,
                                                               FGameplayTagContainer CallTags)
{
	if (!Action_DispatcherEventClass)
	{
		COS_LOG_SCREEN(TEXT("Action_DispatcherEventClass 클래스를 설정해주세요."));
		return;
	}


	FTransform SpawnTransform = FTransform::Identity;

	if (AAction_DispatcherEvent* NewDispatcherEvent = GetWorld()->SpawnActorDeferred<AAction_DispatcherEvent>(Action_DispatcherEventClass, SpawnTransform,
	                                                                                                          nullptr, nullptr,
	                                                                                                          ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
	{
		NewDispatcherEvent->Event = Event;
		NewDispatcherEvent->CallingObject = CallingObject;
		NewDispatcherEvent->DispatcherHub = this;
		NewDispatcherEvent->AlsoCallGlobal = ECallGlobal::CallAfter;
		NewDispatcherEvent->PayLoad = PayLoad;
		NewDispatcherEvent->CallTags = CallTags;
		NewDispatcherEvent->EndDelay = EndDelay;
		NewDispatcherEvent->FinishSpawning(SpawnTransform);
	}
}

bool UDispatcherHubComponent::ResolveCallEvent(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal, UObject* PayLoad, FGameplayTagContainer CallTags)
{
	// EventLists에 EventTag가 존재하지 않으면, true 반환 후 종료
	if (!EventLists.Contains(EventTag))
	{
		return true;
	}

	// 특정 객체에 대해 이벤트를 호출하는 경우
	if (IsValid(CallSpecificObject))
	{
		// CallSpecificObject가 UInterface_EventHolder 인터페이스를 구현하는지 확인
		if (ImplementsEventHolderInterface(CallSpecificObject))
		{
			// CallSpecificObject에서 이벤트 실행
			IInterface_EventHolder::Execute_RunEvent(CallSpecificObject, EventTag, CallingObject, bGlobal, PayLoad, CallTags);
		}
		return true; // CallSpecificObject가 유효하지만 인터페이스를 구현하지 않는 경우
	}

	// EventTag에 해당하는 모든 이벤트 홀더를 가져옴
	const TArray<UObject*>& EventHolderList = EventLists[EventTag].Objects;

	// 유효하지 않은 객체를 추적하기 위한 배열
	TArray<UObject*> InvalidEventHolders;

	// 모든 이벤트 홀더에 대해 반복 처리
	for (UObject* CurrentEventHolder : EventHolderList)
	{
		// 현재 이벤트 홀더가 유효하지 않은 경우
		if (!IsValid(CurrentEventHolder))
		{
			InvalidEventHolders.Add(CurrentEventHolder); // 유효하지 않은 객체를 추적
			continue;
		}

		// 현재 이벤트 홀더가 UInterface_EventHolder 인터페이스를 구현하지 않는 경우
		if (!ImplementsEventHolderInterface(CurrentEventHolder))
		{
			continue; // 다음 이벤트 홀더로 넘어감
		}

		// 이벤트 실행
		IInterface_EventHolder::Execute_RunEvent(CurrentEventHolder, EventTag, CallingObject, bGlobal, PayLoad, CallTags);

		// 이벤트가 인터럽트된 경우 처리
		if (bInterrupt)
		{
			bInterrupt = false; // 인터럽트 상태 초기화
			return true; // 인터럽트가 발생했으므로 종료
		}
	}

	// 유효하지 않은 객체가 없으면 종료
	if (InvalidEventHolders.Num() == 0)
	{
		return true;
	}

	// EventTag에 해당하는 로컬 객체 배열을 업데이트하여 유효하지 않은 객체 제거
	TArray<UObject*>& ValidEventHolderList = EventLists[EventTag].Objects;
	for (UObject* InvalidEventHolder : InvalidEventHolders)
	{
		ValidEventHolderList.Remove(InvalidEventHolder); // 유효하지 않은 객체 제거
	}

	// 업데이트된 객체 배열을 EventLists에 다시 추가
	EventLists.Add(EventTag, FObjectArray(ValidEventHolderList));

	return true; // 모든 작업이 완료되었으므로 true 반환
}

bool UDispatcherHubComponent::CallEventWithPayload(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal, UObject* PayLoad)
{
	return ResolveCallEvent(EventTag, CallingObject, CallSpecificObject, AlsoCallGlobal, PayLoad, FGameplayTagContainer());
}

bool UDispatcherHubComponent::CallEventWithCallTags(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal, FGameplayTagContainer CallTags)
{
	return ResolveCallEvent(EventTag, CallingObject, CallSpecificObject, AlsoCallGlobal, nullptr, CallTags);
}

bool UDispatcherHubComponent::CallEventWithPayloadAndCallTags(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal, UObject* PayLoad,
                                                              FGameplayTagContainer CallTags)
{
	return ResolveCallEvent(EventTag, CallingObject, CallSpecificObject, AlsoCallGlobal, PayLoad, CallTags);
}

bool UDispatcherHubComponent::IsValidGameInstance(UGameInstance* GameInstance)
{
	return GameInstance && GameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass());
}

void UDispatcherHubComponent::AddNewEventHolder(FGameplayTag EventTag, UObject* EventHolder)
{
	FObjectArray NewObjectArray;
	NewObjectArray.Objects = {EventHolder};
	EventLists.Add(EventTag, NewObjectArray);
}

bool UDispatcherHubComponent::ImplementsEventHolderInterface(UObject* EventHolder)
{
	return EventHolder->GetClass()->ImplementsInterface(UInterface_EventHolder::StaticClass());
}

void UDispatcherHubComponent::InsertEventHolderWithPriority(FGameplayTag EventTag, UObject* EventHolder)
{
	// EventHolder의 우선순위를 가져옵니다.
	float HolderPriority = IInterface_EventHolder::Execute_GetPriority(EventHolder);

	// EventTag와 연관된 객체 목록을 가져옵니다.
	TArray<UObject*>& EventObjects = EventLists[EventTag].Objects;
	TArray<UObject*> InvalidObjects; // 무효한 객체들을 저장할 배열입니다.
	bool bInserted = false; // EventHolder가 삽입되었는지 여부를 추적합니다.

	// 현재 목록의 각 객체와 우선순위를 비교합니다.
	for (int32 ObjectIndex = 0; ObjectIndex < EventObjects.Num(); ++ObjectIndex)
	{
		UObject* CurrentObject = EventObjects[ObjectIndex];

		// 현재 객체가 유효하지 않으면 무효한 객체 목록에 추가하고 다음 객체로 이동합니다.
		if (!IsValid(CurrentObject))
		{
			InvalidObjects.AddUnique(CurrentObject);
			continue;
		}

		// 현재 객체가 UInterface_EventHolder 인터페이스를 구현하는지 확인합니다.
		if (!ImplementsEventHolderInterface(CurrentObject))
		{
			continue;
		}

		// 현재 객체의 우선순위를 가져옵니다.
		float CurrentObjectPriority = IInterface_EventHolder::Execute_GetPriority(CurrentObject);

		// 현재 객체의 우선순위가 EventHolder의 우선순위보다 높거나 같으면 그 뒤에 삽입합니다.
		if (CurrentObjectPriority >= HolderPriority)
		{
			EventObjects.Insert(EventHolder, ObjectIndex + 1);
			bInserted = true; // 삽입되었음을 표시합니다.
			break; // 루프를 종료합니다.
		}
	}

	// 삽입되지 않은 경우 가장 앞에 삽입합니다.
	if (!bInserted)
	{
		EventObjects.Insert(EventHolder, 0);
	}

	RemoveInvalidObjects(EventTag, InvalidObjects);
}

void UDispatcherHubComponent::RemoveInvalidObjects(FGameplayTag EventTag, const TArray<UObject*>& InvalidObjects)
{
	// 무효한 객체가 없으면 종료합니다.
	if (InvalidObjects.Num() <= 0)
	{
		return;
	}

	// EventTag에 해당하는 객체 배열을 찾습니다.
	FObjectArray* LocalObjects = EventLists.Find(EventTag);
	if (!LocalObjects)
	{
		return; // 배열을 찾을 수 없으면 함수를 종료합니다.
	}

	// 무효한 객체들을 제거합니다.
	for (UObject* InvalidObject : InvalidObjects)
	{
		LocalObjects->Objects.Remove(InvalidObject);
	}

	// 수정된 배열을 다시 EventLists에 추가합니다.
	EventLists.Add(EventTag, FObjectArray(*LocalObjects));
}

void UDispatcherHubComponent::Interrupt_Implementation()
{
	bInterrupt = true;
}

void UDispatcherHubComponent::CancelInterruption_Implementation()
{
	bInterrupt = false;
}
