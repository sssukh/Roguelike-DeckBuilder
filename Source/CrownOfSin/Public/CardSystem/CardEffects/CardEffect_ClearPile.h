

#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_ClearPile.generated.h"

/* UCardEffect_ClearPile 클래스는 지정된 대상의 카드 더미(Pile)를 제거하는 효과를 처리하는 클래스입니다.
 * 타겟으로 지정된 구성요소가 카드 더미를 구현하는 경우, 해당 더미에 있는 모든 카드를 제거하는 기능을 제공합니다.
 */
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_ClearPile : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	UCardEffect_ClearPile();

protected:
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
