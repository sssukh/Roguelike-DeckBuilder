#pragma once

#include "CoreMinimal.h"
#include "DispatcherHubComponent.h"
#include "DispatcherHubLocalComponent.generated.h"


/*전역적으로 전송될 필요가 없는 이벤트를 위해 카드 및 미니언과 같은 특정 액터에 첨부되도록 고안된 DispatcherHub 버전입니다.
 *조직 및 성능에 유용합니다.
 *글로벌 디스패처 허브는 로컬 허브에 대해 알지 못합니다.
 *로컬 디스패처 허브는 자체 외에도 글로벌 디스패처 허브의 이벤트를 호출하도록 지시받을 수 있습니다.*/
UCLASS(ClassGroup=("Cos"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UDispatcherHubLocalComponent : public UDispatcherHubComponent
{
	GENERATED_BODY()

public:
	UDispatcherHubLocalComponent();

protected:
	virtual void BeginPlay() override;


	/*========================================================================================
	*	Dispatcher Hub Override
	=========================================================================================*/
public:
	virtual void BindEventToHub(UObject* EventHolder, FGameplayTag EventTag) override;

	virtual void UnbindEventFromHub(UObject* EventHolder, FGameplayTag EventTag) override;

	virtual bool CallEvent(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject = nullptr, ECallGlobal AlsoCallGlobal = ECallGlobal::CallAfter) override;

	virtual bool CallEventWithCallTags(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject = nullptr, ECallGlobal AlsoCallGlobal = ECallGlobal::CallAfter,
	                                   FGameplayTagContainer CallTags = FGameplayTagContainer()) override;

	virtual bool CallEventWithPayload(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal, UObject* PayLoad) override;

	virtual bool CallEventWithPayloadAndCallTags(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal, UObject* PayLoad,
	                                             FGameplayTagContainer CallTags) override;

	virtual bool ResolveCallEvent(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal, UObject* PayLoad, FGameplayTagContainer CallTags) override;


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(BlueprintReadWrite, Category="Dispatcher Hub Local")
	UDispatcherHubComponent* GlobalDispatcherHubComponentRef;

	UPROPERTY(BlueprintReadWrite, Category="Dispatcher Hub Local")
	bool bEmpty = true;
};
