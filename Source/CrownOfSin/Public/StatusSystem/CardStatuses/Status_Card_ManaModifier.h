#pragma once

#include "CoreMinimal.h"
#include "StatusSystem/StatusComponent.h"
#include "Status_Card_ManaModifier.generated.h"

class ACardBase;

/*소유한 카드의 마나 비용을 수정합니다.*/
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Card_ManaModifier : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Card_ManaModifier();

protected:
	virtual void BeginPlay() override;

public:
	void ModifyCost();

	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

public:
	UPROPERTY(BlueprintReadWrite,Category="Status Card ManaModifier")
	TObjectPtr<ACardBase> OwningCard;
};
