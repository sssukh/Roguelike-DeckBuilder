
#pragma once

#include "CoreMinimal.h"
#include "StatusSystem/StatusComponent.h"
#include "Status_Card_InterruptOnPlayerUse.generated.h"

/*이 카드가 사용 중일 때 모든 카드 효과가 실행되는 것을 방지합니다.
 *플레이 시 효과를 실행해서는 안 되는 자동 플레이 카드에 사용됩니다(일부 저주 등).*/
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Card_InterruptOnPlayerUse : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Card_InterruptOnPlayerUse();

protected:
	virtual void BeginPlay() override;

public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;
};
