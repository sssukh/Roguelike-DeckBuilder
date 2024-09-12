﻿#include "Core/DispatcherHubComponent.h"


#include "CardSystem/CardActions/Action_DispatcherEvent.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Interfaces/Interface_EventHolder.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


UDispatcherHubComponent::UDispatcherHubComponent()
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

void UDispatcherHubComponent::BindEventToHub(UObject* EventHolder, FGameplayTag EventTag)
{
	if (!IsValid(EventHolder)) return;

	UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
	if (!CardGameInstance) return;

	// 디버그 모드가 활성화되어 있는지 확인합니다.
	if (IInterface_CardGameInstance::Execute_IsDebugMode(CardGameInstance))
	{
		UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_Debug_DispatcherHubUpdate, this);
	}

	// EventTag가 존재하지 않으면 새로 생성하고, EventHolder를 추가합니다.
	if (!EventLists.Contains(EventTag))
	{
		AddNewEventHolder(EventTag, EventHolder);
		return;
	}

	// EventHolder가 이미 등록되어 있는지 확인합니다.  이미 등록된 경우 함수를 종료합니다.	
	if (EventLists[EventTag].Objects.Contains(EventHolder)) return;

	// EventHolder가 UInterface_EventHolder 인터페이스를 구현하는지 확인합니다. 구현하지 않으면 함수를 종료합니다.
	if (!EventHolder->Implements<UInterface_EventHolder>()) return;

	// 우선순위에 따라 EventHolder를 삽입합니다.
	InsertEventHolderWithPriority(EventTag, EventHolder);
}

void UDispatcherHubComponent::BindMultipleEventsToHub(UObject* EventHolder, FGameplayTagContainer EventTags)
{
	// EventTags의 모든 태그를 배열로 변환합니다.
	TArray<FGameplayTag> EventTagList;
	EventTags.GetGameplayTagArray(EventTagList);

	// 변환된 배열의 각 태그에 대해 BindEventToHub 함수를 호출하여 바인딩을 처리합니다.
	for (const FGameplayTag& EventTag : EventTagList)
	{
		BindEventToHub(EventHolder, EventTag);
	}
}

void UDispatcherHubComponent::UnbindEventFromHub(UObject* EventHolder, FGameplayTag EventTag)
{
	// 1. 이벤트 리스트에 해당 EventTag가 존재하는지 확인. EventTag가 없으면 함수 종료.
	if (!EventLists.Contains(EventTag)) return;

	// 2. 해당 EventTag에 바인딩된 이벤트 홀더 목록을 가져옴.
	TArray<UObject*>& BoundEventHolders = EventLists[EventTag].Objects;

	// 3. 이벤트 홀더 목록에서 주어진 EventHolder를 제거. 제거된 수를 반환.
	int32 RemovedCount = BoundEventHolders.Remove(EventHolder);

	// 4. 제거된 이벤트 홀더가 없으면 함수 종료.
	if (RemovedCount == 0) return;

	// 5. 이벤트 홀더 목록을 업데이트하여 EventLists에 다시 추가.
	EventLists[EventTag] = BoundEventHolders;

	// 6. 디버그 모드가 활성화된 경우, 디버그 업데이트 이벤트를 호출하여 상태를 업데이트.
	UFunctionLibrary_Event::CallEventInGlobalDispatcherHub(CosGameTags::Event_Debug_DispatcherHubUpdate, this);
}

void UDispatcherHubComponent::UnbindMultipleEventsFromHub(UObject* EventHolder, FGameplayTagContainer EventTags)
{
	// 1. EventTags에서 개별 이벤트 태그들을 가져와 배열로 변환.
	TArray<FGameplayTag> EventTagList;
	EventTags.GetGameplayTagArray(EventTagList);

	// 2. 각 이벤트 태그에 대해 UnbindEventFromHub를 호출하여 이벤트 홀더 해제.
	for (const FGameplayTag& EventTag : EventTagList)
	{
		UnbindEventFromHub(EventHolder, EventTag);
	}
}

bool UDispatcherHubComponent::CallEvent(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal)
{
	return ResolveCallEvent(EventTag, CallingObject, CallSpecificObject, AlsoCallGlobal);
}

bool UDispatcherHubComponent::CallMultipleBoundEvents(const TArray<FGameplayTag>& EventTagsGameplayTags, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal)
{
	for (const FGameplayTag& EventTag : EventTagsGameplayTags)
	{
		ResolveCallEvent(EventTag, CallingObject, CallSpecificObject, AlsoCallGlobal);
	}
	return true;
}

bool UDispatcherHubComponent::ResolveCallEvent(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal, UObject* PayLoad, FGameplayTagContainer CallTags)
{
	// 1. EventTag에 해당하는 이벤트가 없으면 바로 종료.
	if (!EventLists.Contains(EventTag)) return true;

	// 2. 특정 객체에 대해 이벤트를 호출하는 경우.
	if (IsValid(CallSpecificObject))
	{
		// CallSpecificObject가 UInterface_EventHolder 인터페이스를 구현하는지 확인.
		if (CallSpecificObject->Implements<UInterface_EventHolder>())
		{
			// 해당 객체에서 이벤트를 실행.
			IInterface_EventHolder::Execute_RunEvent(CallSpecificObject, EventTag, CallingObject, bGlobal, PayLoad, CallTags);
		}
		return true; // 특정 객체에 대해서만 이벤트를 호출했으므로 종료.
	}

	// 3. EventTag에 바인딩된 모든 이벤트 홀더를 가져옴.
	const TArray<UObject*>& EventHolderList = EventLists[EventTag].Objects;

	// 유효하지 않은 이벤트 홀더를 저장할 배열.
	TArray<UObject*> InvalidEventHolders;

	// 4. 바인딩된 모든 이벤트 홀더에 대해 반복 처리.
	for (UObject* CurrentEventHolder : EventHolderList)
	{
		// 이벤트 홀더가 유효하지 않은 경우, 추적 리스트에 추가.
		if (!IsValid(CurrentEventHolder))
		{
			InvalidEventHolders.Add(CurrentEventHolder);
			continue;
		}

		// 이벤트 홀더가 UInterface_EventHolder 인터페이스를 구현하지 않으면 건너뜀.
		if (!CurrentEventHolder->Implements<UInterface_EventHolder>()) continue;

		// 5. 이벤트를 실행.
		IInterface_EventHolder::Execute_RunEvent(CurrentEventHolder, EventTag, CallingObject, bGlobal, PayLoad, CallTags);

		// 6. 인터럽트가 발생한 경우, 처리 후 종료.
		if (bInterrupt)
		{
			bInterrupt = false; // 인터럽트 상태 초기화.
			return true; // 인터럽트 발생 시 종료.
		}
	}

	// 7. 유효하지 않은 이벤트 홀더가 없으면 바로 종료.
	if (InvalidEventHolders.Num() <= 0) return true;

	// 8. 유효하지 않은 객체들을 제거하여 이벤트 홀더 리스트를 정리.
	TArray<UObject*>& ValidEventHolderList = EventLists[EventTag].Objects;
	for (UObject* InvalidEventHolder : InvalidEventHolders)
	{
		ValidEventHolderList.Remove(InvalidEventHolder); // 유효하지 않은 객체 제거.
	}

	// 9. 정리된 리스트를 EventLists에 다시 추가.
	EventLists[EventTag] = ValidEventHolderList;

	return true; // 모든 작업 완료 후 true 반환.
}

void UDispatcherHubComponent::QueueEvent(FGameplayTag Event, UObject* CallingObject, UObject* CallSpecificObject, float EndDelay, UObject* PayLoad, FGameplayTagContainer CallTags)
{
	// 액터 클래스가 설정되지 않은 경우 로그 메시지를 출력하고 함수 종료.
	if (!Action_DispatcherEventClass)
	{
		COS_LOG_SCREEN(TEXT("Action_DispatcherEventClass 클래스를 설정해주세요."));
		return;
	}

	// 액터가 생성될 위치 및 회전을 나타내는 기본 변환 값을 설정.
	FTransform SpawnTransform = FTransform::Identity;

	// 지연 생성 방식으로 AAction_DispatcherEvent 액터를 생성. 액터 생성이 성공한 경우에만 이후 로직을 실행.
	AAction_DispatcherEvent* NewDispatcherEvent = GetWorld()->SpawnActorDeferred<AAction_DispatcherEvent>(
		Action_DispatcherEventClass,
		SpawnTransform,
		nullptr, nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (NewDispatcherEvent)
	{
		// 생성된 액터에 이벤트와 관련된 속성들을 설정.
		NewDispatcherEvent->Event = Event;
		NewDispatcherEvent->CallingObject = CallingObject;
		NewDispatcherEvent->DispatcherHub = this;
		NewDispatcherEvent->AlsoCallGlobal = ECallGlobal::CallAfter; // 전역 이벤트 호출 여부 설정.
		NewDispatcherEvent->PayLoad = PayLoad;
		NewDispatcherEvent->CallTags = CallTags;
		NewDispatcherEvent->EndDelay = EndDelay;

		// 액터의 생성이 완료됨을 알림.
		NewDispatcherEvent->FinishSpawning(SpawnTransform);
	}
}

void UDispatcherHubComponent::QueueEventWithPayloadAndCallTags(FGameplayTag Event, UObject* CallingObject, UObject* CallSpecificObject, float EndDelay, UObject* PayLoad,
                                                               FGameplayTagContainer CallTags)
{
	// 이벤트 액터 클래스가 설정되지 않았을 경우, 경고 메시지를 출력하고 함수를 종료합니다.
	if (!Action_DispatcherEventClass)
	{
		COS_LOG_SCREEN(TEXT("Action_DispatcherEventClass 클래스를 설정해주세요."));
		return;
	}

	// 액터가 생성될 위치와 회전을 나타내는 기본 변환 값을 설정합니다.
	FTransform SpawnTransform = FTransform::Identity;

	// AAction_DispatcherEvent 액터를 지연 생성. 액터가 성공적으로 생성된 경우에만 이후 로직을 실행합니다.
	AAction_DispatcherEvent* NewDispatcherEvent = GetWorld()->SpawnActorDeferred<AAction_DispatcherEvent>(
		Action_DispatcherEventClass, // 생성할 액터 클래스
		SpawnTransform, // 액터의 스폰 변환 정보
		nullptr, // 소유자 (필요 시 설정)
		nullptr, // 인스턴스화된 Pawn이나 캐릭터 (필요 시 설정)
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn // 충돌 핸들링 방법
	);

	// 액터가 성공적으로 생성되었다면 필요한 속성들을 설정합니다.
	if (NewDispatcherEvent)
	{
		NewDispatcherEvent->Event = Event;
		NewDispatcherEvent->CallingObject = CallingObject;
		NewDispatcherEvent->CallSpecificObject = CallSpecificObject;
		NewDispatcherEvent->DispatcherHub = this;
		NewDispatcherEvent->AlsoCallGlobal = ECallGlobal::CallAfter; // 전역 이벤트 호출 여부 설정
		NewDispatcherEvent->PayLoad = PayLoad;
		NewDispatcherEvent->CallTags = CallTags;
		NewDispatcherEvent->EndDelay = EndDelay;

		// 액터 생성이 완료됨을 알립니다.
		NewDispatcherEvent->FinishSpawning(SpawnTransform);
	}
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

void UDispatcherHubComponent::AddNewEventHolder(FGameplayTag EventTag, UObject* EventHolder)
{
	FObjectArray NewObjectArray;
	NewObjectArray.Objects = {EventHolder};
	EventLists.Add(EventTag, NewObjectArray);
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
		if (!CurrentObject->Implements<UInterface_EventHolder>()) continue;

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
	if (InvalidObjects.Num() <= 0) return;

	// EventTag에 해당하는 객체 배열을 찾습니다.
	FObjectArray* LocalObjects = EventLists.Find(EventTag);
	if (!LocalObjects) return; // 배열을 찾을 수 없으면 함수를 종료합니다.

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
