
#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_Guarded.generated.h"

class AMinionBase;

/*소유한 미니언은 같은 미니언 트랙에 다른 미니언이 남아 있는 한 절반의 피해를 입습니다.*/
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Guarded : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Guarded();

protected:
	virtual void BeginPlay() override;

public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;
public:
	UPROPERTY(BlueprintReadWrite,Category="Status Guarded")
	TObjectPtr<AMinionBase> OwningMinion;
};
