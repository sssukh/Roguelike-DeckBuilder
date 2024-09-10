
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_AddStatusFromDrawPileSize.generated.h"

/*카드 플레이어의 뽑기 더미에 있는 카드 수와 동일한 상태 값을 사용하여 대상 구성 요소에 지정된 상태를 대상에 추가합니다.
 *대상이 이미 상태를 갖고 있는 경우 효과 값은 기존 상태에 추가됩니다(또는 부정적인 효과 값의 경우 뺍니다).*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_AddStatusFromDrawPileSize : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCardEffect_AddStatusFromDrawPileSize();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
