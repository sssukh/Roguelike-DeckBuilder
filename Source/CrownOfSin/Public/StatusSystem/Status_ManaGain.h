
#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_ManaGain.generated.h"

/* 
 * UStatus_ManaGain 클래스는 카드 플레이어가 보유하는 마나 획득 시스템을 관리하는 상태 컴포넌트입니다.
 * 이 컴포넌트는 매 턴이 시작될 때 마나를 갱신하며, BP_Status_Mana에 ManaGain 값으로 마나를 설정합니다.
 * 턴이 시작될 때 Event_TurnStart 이벤트를 수신하여, Mana 상태 값을 업데이트하고 이벤트를 전파합니다.
 */
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_ManaGain : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_ManaGain();

protected:
	virtual void BeginPlay() override;

public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;
};
