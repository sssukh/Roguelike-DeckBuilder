#pragma once

#include "CoreMinimal.h"
#include "StatusSystem/StatusComponent.h"
#include "Status_Card_ManaReduceOnDraw.generated.h"

class ACardBase;

/*이 카드가 플레이어의 손에 있는 동안 다른 카드를 뽑을 때마다 이 카드의 마나 비용이 감소합니다.*/
UCLASS(ClassGroup=("COS|Status"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UStatus_Card_ManaReduceOnDraw : public UStatusComponent
{
	GENERATED_BODY()

public:
	UStatus_Card_ManaReduceOnDraw();

protected:
	virtual void BeginPlay() override;

public:
	virtual void RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal, UObject* PayLoad, const FGameplayTagContainer& CallTags) override;

public:
	void ApplyManaCostReduction();
	
public:
	UPROPERTY(BlueprintReadWrite, Category="Status Card ManaReduceOnDraw")
	int32 CurrentModification = 0;
	
	UPROPERTY(BlueprintReadWrite, Category="Status Card ManaReduceOnDraw")
	TObjectPtr<ACardBase> OwningCard;
};
