#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_Fear.generated.h"

/*이 상태가 해제될 때까지 이 상태의 미니언이 소유한 공격 태그가 붙은 카드 내 공격 태그 효과의 효과 값이 절반으로 감소합니다.
 *상태 값은 남은 턴 수를 결정합니다.*/
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Fear : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Fear();

protected:
	virtual void BeginPlay() override;

public:
	virtual int32 AddStatusValue(int32 InAmount, bool bShowSplashNumber, bool bShowSplashIcon, bool bRefreshAppearance, UObject* InPayload) override;
	
public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;
};
