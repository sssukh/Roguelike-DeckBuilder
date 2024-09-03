#pragma once

#include "CoreMinimal.h"
#include "CosEnumStruct.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Interface_Interrupt.h"
#include "DispatcherHubComponent.generated.h"

/*블루프린트 전체에서 이벤트 바인딩 및 호출을 처리합니다.
 *적절한 이벤트가 트리거될 때 호출될 액터의 정렬되고 우선순위가 지정된 목록을 보유합니다.*/
UCLASS(ClassGroup=("Cos"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UDispatcherHubComponent : public UActorComponent, public IInterface_Interrupt
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDispatcherHubComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void BindEventToHub(UObject* EventHolder, FGameplayTag EventTag);
	


	//ToDo::구현해야함
	bool CallEvent(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal) { return false; }

	//ToDo::구현해야함
	bool CallEventWithCallTags(FGameplayTag EventTag, UObject* CallingObject, UObject* CallSpecificObject, ECallGlobal AlsoCallGlobal,FGameplayTagContainer CallTags){return false;}
	

protected:
	bool IsValidEventHolder(UObject* EventHolder);

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
	UPROPERTY(BlueprintReadWrite, Category="Dispatcher Hub")
	bool bInterrupt;

	UPROPERTY(BlueprintReadWrite, Category="Dispatcher Hub")
	TMap<FGameplayTag, FObjectArray> EventLists;
};
