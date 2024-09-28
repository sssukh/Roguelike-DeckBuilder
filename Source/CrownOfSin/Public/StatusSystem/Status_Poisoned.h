
#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_Poisoned.generated.h"

/*소유한 하수인은 턴 종료 시 상태값만큼 피해를 입고, 그 후 상태값이 1 감소합니다.*/
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Poisoned : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Poisoned();

protected:
	virtual void BeginPlay() override;

public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;
};
