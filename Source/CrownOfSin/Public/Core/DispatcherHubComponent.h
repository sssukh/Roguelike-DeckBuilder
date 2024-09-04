#pragma once

#include "CoreMinimal.h"
#include "CosEnumStruct.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Interface_Interrupt.h"
#include "DispatcherHubComponent.generated.h"

class AAction_DispatcherEvent;


/*블루프린트 전체에서 이벤트 바인딩 및 호출을 처리합니다.
 *적절한 이벤트가 트리거될 때 호출될 액터의 정렬되고 우선순위가 지정된 목록을 보유합니다.*/
UCLASS(ClassGroup=("Cos"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UDispatcherHubComponent : public UActorComponent, public IInterface_Interrupt
{
	GENERATED_BODY()

public:
	UDispatcherHubComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// EventHolder를 지정된 EventTag에 바인딩하고, 우선순위에 따라 EventHolder를 정렬합니다.
	// 게임 인스턴스와 이벤트 홀더의 유효성을 확인하고, 올바른 위치에 삽입하여 이벤트 시스템을 관리합니다.
	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	virtual void BindEventToHub(UObject* EventHolder, FGameplayTag EventTag);

	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	void BindMultipleEventsToHub(UObject* EventHolder, FGameplayTagContainer EventTags);

	// 특정 EventTag에 바인딩된 EventHolder를 EventLists에서 제거합니다.
	// 이벤트 태그에 연결된 객체를 제거하고, 디버그 업데이트 이벤트를 호출합니다.
	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	virtual void UnbindEventFromHub(UObject* EventHolder, FGameplayTag EventTag);


	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	void UnbindMultipleEventsFromHub(UObject* EventHolder, FGameplayTagContainer EventTags);

	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	bool CallMultipleBoundEvents(const TArray<FGameplayTag>& EventTagsGameplayTags, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal);

	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	virtual bool CallEvent(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject = nullptr, ECallGlobal AlsoCallGlobal = ECallGlobal::CallAfter);

	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	void QueueEvent(FGameplayTag Event, UObject* CallingObject, UObject* CallSpecificObject, float EndDelay, UObject* PayLoad, FGameplayTagContainer CallTags);

	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	void QueueEventWithPayloadAndCallTags(FGameplayTag Event, UObject* CallingObject, UObject* CallSpecificObject, float EndDelay, UObject* PayLoad, FGameplayTagContainer CallTags);

	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	virtual bool ResolveCallEvent(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal, UObject* PayLoad = nullptr,
	                              FGameplayTagContainer CallTags = FGameplayTagContainer());

	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	virtual bool CallEventWithPayload(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal, UObject* PayLoad);

	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	virtual bool CallEventWithCallTags(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal, FGameplayTagContainer CallTags);

	UFUNCTION(BlueprintCallable, Category="Dispatcher Hub Event")
	virtual bool CallEventWithPayloadAndCallTags(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal, UObject* PayLoad,
	                                             FGameplayTagContainer CallTags);

protected:
	// EventHolder가 UInterface_EventHolder 인터페이스를 구현하는지 확인합니다.
	// 유효성을 검사하여, 인터페이스를 상속받았는지 여부를 반환합니다.
	void AddNewEventHolder(FGameplayTag EventTag, UObject* EventHolder);

	// EventTag에 해당하는 EventHolder를 우선순위에 따라 적절한 위치에 삽입합니다.
	// 기존 객체들과 우선순위를 비교하여 올바른 위치에 삽입하며, 무효한 객체는 제거합니다.
	void InsertEventHolderWithPriority(FGameplayTag EventTag, UObject* EventHolder);

	// EventTag에 연관된 무효한 객체들을 EventLists에서 제거합니다.
	// 유효하지 않은 객체를 식별하여 목록에서 제거하여 목록의 무결성을 유지합니다.
	void RemoveInvalidObjects(FGameplayTag EventTag, const TArray<UObject*>& InvalidObjects);

public:
	// GameInstance가 UInterface_CardGameInstance 인터페이스를 구현하는지 확인합니다.
	// 유효성을 검사하여, 인터페이스를 상속받았는지 여부를 반환합니다.
	bool IsValidGameInstance(UGameInstance* GameInstance);

	// 새로운 EventTag와 연결된 EventHolder를 EventLists에 추가합니다.
	// 새로운 태그와 이벤트 홀더 쌍을 초기화하고 목록에 삽입합니다.
	bool ImplementsEventHolderInterface(UObject* EventHolder);


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
	UPROPERTY(BlueprintReadWrite, Category="Dispatcher Hub|Config|SetUp")
	TSubclassOf<AAction_DispatcherEvent> Action_DispatcherEventClass;

	UPROPERTY(BlueprintReadWrite, Category="Dispatcher Hub")
	bool bInterrupt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Dispatcher Hub")
	bool bGlobal;

	UPROPERTY(BlueprintReadWrite, Category="Dispatcher Hub")
	TMap<FGameplayTag, FObjectArray> EventLists;

	UPROPERTY(BlueprintReadWrite, Category="Dispatcher Hub")
	FGameplayTagContainer BlankContainer;
};
