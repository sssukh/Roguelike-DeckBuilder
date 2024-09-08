
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_StoreTargetAsPayload.generated.h"

/*이 카드 효과의 대상을 카드 플레이어의 페이로드 홀더 구성 요소에 페이로드로 저장합니다.
 *이는 이후 카드 효과에서 저장된 대상을 타겟팅하기 위해 BP_Targeting_PayloadWithIdentifier와 함께 사용할 수 있습니다.
 *Targeting_SameAsPrevious가 충분하지 않은 경우에 사용할 수 있습니다.
 *저장/액세스할 특정 페이로드를 지정하려면 식별자가 필요합니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_StoreTargetAsPayload : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	UCardEffect_StoreTargetAsPayload();

protected:
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;

};
