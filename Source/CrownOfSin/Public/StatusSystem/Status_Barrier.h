#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_Barrier.generated.h"


/*소유한 미니언에 대한 공격으로 인한 피해를 무효화하고 보호막 효과 값을 1만큼 감소시킵니다.*/
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Barrier : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Barrier();

protected:
	virtual void BeginPlay() override;

public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

};
