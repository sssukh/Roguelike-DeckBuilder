#pragma once

#include "CoreMinimal.h"
#include "StatusSystem/StatusComponent.h"
#include "Utilities/FlowControllerLibiary.h"
#include "Status_Card_PlayCardOnCardPlayerEvent.generated.h"

/*카드 플레이어의 이벤트 디스패처에서 지정된 게임플레이 태그 이벤트가 호출되면 자동으로 이 카드를 재생합니다.*/
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Card_PlayCardOnCardPlayerEvent : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Card_PlayCardOnCardPlayerEvent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;


public:
	FGate RunEventGate =FGate(true);
};
