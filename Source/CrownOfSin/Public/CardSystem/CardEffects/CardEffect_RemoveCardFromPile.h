#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_RemoveCardFromPile.generated.h"


class UPileComponent;
/*타겟 컴포넌트에서 설정한 파일컴포넌트에서 대상 카드를 제거합니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_RemoveCardFromPile : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	UCardEffect_RemoveCardFromPile();

protected:
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
	
};
