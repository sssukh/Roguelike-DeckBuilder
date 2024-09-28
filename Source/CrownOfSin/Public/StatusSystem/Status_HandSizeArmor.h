
#pragma once

#include "CoreMinimal.h"
#include "StatusComponent.h"
#include "Status_HandSizeArmor.generated.h"

/*턴 종료 시 플레이어의 손에 남아 있는 카드 수만큼 소유자에게 방어구를 부여합니다.*/
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_HandSizeArmor : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_HandSizeArmor();

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;
};
