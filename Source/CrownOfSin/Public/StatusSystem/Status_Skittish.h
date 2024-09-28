#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_Skittish.generated.h"

/*소유한 하수인은 매 턴마다 처음으로 피해를 입을 때 공포 중첩 1개를 얻습니다.*/
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Skittish : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Skittish();

protected:
	virtual void BeginPlay() override;

public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

private:
	bool bDoOnce = true;
};
