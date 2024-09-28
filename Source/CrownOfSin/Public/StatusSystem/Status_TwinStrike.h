
#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_TwinStrike.generated.h"

class ACardBase;
/*다음에 사용되는 카드에 +X 반복을 추가합니다.*/
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_TwinStrike : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_TwinStrike();

protected:
	virtual void BeginPlay() override;

public:
	virtual  void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

public:
	UPROPERTY(BlueprintReadWrite,Category="Status TwinStrike")
	TObjectPtr<ACardBase> AffectedCard;
};
