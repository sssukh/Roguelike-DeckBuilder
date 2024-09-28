#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_DrawOnDiscard.generated.h"

/*플레이어가 적극적으로 카드를 버릴 때마다 카드 플레이어가 카드를 뽑게 만드는 상태입니다.*/
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_DrawOnDiscard : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_DrawOnDiscard();

protected:
	virtual void BeginPlay() override;

public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;
};
