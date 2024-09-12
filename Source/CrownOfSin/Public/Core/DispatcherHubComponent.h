#pragma once

#include "CoreMinimal.h"
#include "CosEnumStruct.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Interface_Interrupt.h"
#include "DispatcherHubComponent.generated.h"

class AAction_DispatcherEvent;

/*
 * UDispatcherHubComponent는 게임 내 다양한 이벤트 시스템을 관리하고 조율하는 핵심 컴포넌트입니다.
 * 이 컴포넌트는 각 객체가 특정 이벤트에 반응할 수 있도록 이벤트를 바인딩하고 호출하는 역할을 합니다.
 * 이벤트 홀더 객체들은 우선순위에 따라 정렬되며, 이벤트 호출 시 우선순위에 따라 처리됩니다.
 * 또한, 이벤트 큐잉 시스템을 통해 이벤트의 지연 호출이 가능합니다.
 * 이 컴포넌트는 다양한 게임플레이 태그(EventTag)를 활용하여 각기 다른 이벤트들을 그룹화하고 관리합니다.
 *
 * 주요 기능:
 * 1. 이벤트 바인딩 및 해제: 각 객체(EventHolder)를 특정 게임플레이 태그에 바인딩하거나 해제할 수 있습니다.
 * 2. 이벤트 호출: 바인딩된 이벤트를 태그 기반으로 호출하고, 필요한 경우 페이로드(Payload)를 전달할 수 있습니다.
 * 3. 큐잉된 이벤트 호출: 특정 시간이 지난 후 이벤트를 호출할 수 있도록 큐에 등록할 수 있습니다.
 * 4. 인터럽트 시스템: 이벤트 처리 중 특정 조건에 따라 중단할 수 있으며, 중단 상태를 해제할 수 있습니다.
 * 5. 유효성 검사 및 정리: 유효하지 않은 객체들을 식별하고, 이벤트 시스템에서 자동으로 제거하여 관리합니다.
 * 6. 우선순위 기반 이벤트 처리: 이벤트 홀더들이 우선순위에 따라 정렬되고, 이벤트 호출 시 해당 우선순위에 맞게 처리됩니다.
 * 
 * 이 컴포넌트는 IInterface_Interrupt 인터페이스를 구현하여, 인터럽트 및 인터럽트 취소 기능을 제공합니다.
 */
UCLASS(ClassGroup=("Cos|Core"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UDispatcherHubComponent : public UActorComponent, public IInterface_Interrupt
{
	GENERATED_BODY()

public:
	UDispatcherHubComponent();

protected:
	virtual void BeginPlay() override;

public:
	/*
	 * 이벤트 홀더를 이벤트 허브에 바인딩하는 함수.
	 * 주어진 이벤트 태그(EventTag)에 해당하는 이벤트 홀더(EventHolder)를 이벤트 리스트에 추가합니다.
	 * EventHolder가 이미 등록되었거나 유효하지 않은 경우 함수를 종료하며, 등록 시 우선순위에 따라 리스트에 삽입합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	virtual void BindEventToHub(UObject* EventHolder, FGameplayTag EventTag);

	/*
	 * 이벤트 홀더를 다수의 이벤트 태그에 바인딩하는 함수.
	 * 주어진 이벤트 태그 컨테이너(EventTags)의 모든 태그에 대해 해당 이벤트 홀더(EventHolder)를 이벤트 허브에 바인딩합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	void BindMultipleEventsToHub(UObject* EventHolder, FGameplayTagContainer EventTags);

	/*
	 * 지정된 이벤트 태그(EventTag)에 바인딩된 이벤트 홀더(EventHolder)를 이벤트 허브에서 제거하는 함수.
	 * 이벤트 태그에 연결된 이벤트 홀더 목록에서 주어진 이벤트 홀더를 제거하고, 디버그 모드가 활성화된 경우 업데이트 이벤트를 호출합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	virtual void UnbindEventFromHub(UObject* EventHolder, FGameplayTag EventTag);

	/*
	 * 주어진 EventTags에 속한 여러 이벤트 태그에서 EventHolder를 해제하는 함수.
	 * 각 이벤트 태그에 대해 UnbindEventFromHub를 호출하여 이벤트 홀더를 EventLists에서 제거합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	void UnbindMultipleEventsFromHub(UObject* EventHolder, FGameplayTagContainer EventTags);

	/*단일 이벤트 호출을 처리하는 함수. 주어진 EventTag에 바인딩된 이벤트를 호출하고, 특정 객체에 대해 호출할 수 있습니다.*/
	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	virtual bool CallEvent(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject = nullptr, ECallGlobal AlsoCallGlobal = ECallGlobal::CallAfter);

	/*여러 이벤트 태그에 바인딩된 이벤트들을 순차적으로 호출하는 함수. 주어진 태그 목록에 있는 모든 이벤트를 호출합니다.*/
	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	bool CallMultipleBoundEvents(const TArray<FGameplayTag>& EventTagsGameplayTags, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal);

	/**
	 * 주어진 이벤트와 관련된 AAction_DispatcherEvent 액터를 지연 생성하여 이벤트 큐에 추가하는 함수.
	 * 이벤트를 처리할 때 사용할 페이로드, 호출 객체, 종료 지연 시간 및 태그를 설정합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	void QueueEvent(FGameplayTag Event, UObject* CallingObject, UObject* CallSpecificObject, float EndDelay, UObject* PayLoad, FGameplayTagContainer CallTags);

	/**
	 * 주어진 이벤트에 대해 페이로드와 태그를 포함하는 AAction_DispatcherEvent 액터를 지연 생성하여 큐에 추가합니다.
	 * 이벤트가 발생할 때 처리할 페이로드와 관련 태그들을 설정하고, 지정된 종료 지연 시간 동안 대기합니다.
	 */
	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	void QueueEventWithPayloadAndCallTags(FGameplayTag Event, UObject* CallingObject, UObject* CallSpecificObject, float EndDelay, UObject* PayLoad, FGameplayTagContainer CallTags);

	/*페이로드를 포함한 이벤트를 호출합니다.*/
	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	virtual bool CallEventWithPayload(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal, UObject* PayLoad);

	/*호출 태그를 포함한 이벤트를 호출합니다.*/
	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	virtual bool CallEventWithCallTags(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject = nullptr, ECallGlobal AlsoCallGlobal = ECallGlobal::CallAfter,
	                                   FGameplayTagContainer CallTags = FGameplayTagContainer());

	/*페이로드와 호출 태그를 모두 포함한 이벤트를 호출합니다.*/
	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	virtual bool CallEventWithPayloadAndCallTags(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal, UObject* PayLoad,
	                                             FGameplayTagContainer CallTags);

	/*
	 * 주어진 EventTag와 관련된 이벤트를 호출하고, 인터럽트가 발생하거나 유효하지 않은 객체들을 정리하는 함수.
	 * 특정 객체에 대해 이벤트를 호출하거나, 이벤트에 바인딩된 모든 객체에 대해 이벤트를 호출합니다.
	 */
	virtual bool ResolveCallEvent(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject = nullptr, ECallGlobal AlsoCallGlobal = ECallGlobal::CallAfter, UObject* PayLoad = nullptr,
	                              FGameplayTagContainer CallTags = FGameplayTagContainer());

protected:
	/*
	 * 새로운 이벤트 태그(EventTag)와 이벤트 홀더(EventHolder)를 EventLists에 추가하는 함수.
	 * 주어진 이벤트 태그에 대응하는 객체 배열을 생성하고, 해당 이벤트 홀더를 리스트에 추가하여 관리합니다.
	 */
	void AddNewEventHolder(FGameplayTag EventTag, UObject* EventHolder);

	/*
	 * 이벤트 홀더를 우선순위에 따라 이벤트 리스트에 삽입하고, 무효한 객체를 제거하는 함수.
	 * 현재 이벤트 리스트에 있는 객체들과 우선순위를 비교하여 적절한 위치에 EventHolder를 삽입합니다.
	 * 또한, 유효하지 않은 객체들은 목록에서 제거합니다.
	 */
	void InsertEventHolderWithPriority(FGameplayTag EventTag, UObject* EventHolder);

	/*
	 * 유효하지 않은 객체들을 이벤트 리스트에서 제거하는 함수.
	 * 무효한 객체 목록을 받아 해당 객체들을 EventLists에서 제거합니다.
	 */
	void RemoveInvalidObjects(FGameplayTag EventTag, const TArray<UObject*>& InvalidObjects);


	/*========================================================================================
	*	IInterface_Interrupt
	=========================================================================================*/
public:
	virtual void Interrupt_Implementation() override;

	virtual void CancelInterruption_Implementation() override;


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispatcher Hub|Config")
	bool bGlobal=false;

	UPROPERTY(BlueprintReadWrite, Category="Dispatcher Hub|Config|SetUp")
	TSubclassOf<AAction_DispatcherEvent> Action_DispatcherEventClass;

	UPROPERTY(BlueprintReadWrite, Category="Dispatcher Hub")
	bool bInterrupt=false;

	UPROPERTY(BlueprintReadWrite, Category="Dispatcher Hub")
	TMap<FGameplayTag, FObjectArray> EventLists;

public:
	FGameplayTagContainer BlankContainer;
};
