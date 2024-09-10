
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_AddToAllStatusesWithTags.generated.h"

/*이 카드 효과의 GameplayTag를 공유하는 대상에서 모든 상태를 찾고 거기에서 지정된 효과 값을 더하거나 뺍니다. 사용 예: 대상의 모든 디버프에서 1을 뺍니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_AddToAllStatusesWithTags : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	
	UCardEffect_AddToAllStatusesWithTags();

protected:
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
