
#pragma once

#include "CoreMinimal.h"
#include "StatusSystem/StatusComponent.h"
#include "Status_Card_RepeatsByManaSpent.generated.h"

class ACardBase;
/*플레이어가 남은 마나 양에 따라 카드에 +X 반복을 추가하고 카드 플레이어의 남은 마나에 대해 카드 비용을 늘립니다.*/
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Card_RepeatsByManaSpent : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Card_RepeatsByManaSpent();

protected:
	virtual void BeginPlay() override;

public:
	void UpdateFromMana(int32 NewMana);
	
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

public:

	UPROPERTY(BlueprintReadWrite,Category="Status Card RepeatsByManaSpent")
	TObjectPtr<ACardBase> OwningCard;
};
