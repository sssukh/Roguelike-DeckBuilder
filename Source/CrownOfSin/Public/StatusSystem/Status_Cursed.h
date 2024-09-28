
#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_Cursed.generated.h"

/*피해 감소를 고려한 후 소유 하수인이 받는 피해가 두 배로 늘어납니다. 매 턴마다 1씩 감소합니다.*/
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Cursed : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Cursed();

protected:
	virtual void BeginPlay() override;

public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;
};
