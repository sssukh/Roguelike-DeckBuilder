#pragma once

#include "CoreMinimal.h"
#include "CardSystem/CardEffects/CardEffectComponent.h"
#include "CardEffect_Story_ModHeroHealth.generated.h"


// 게임 인스턴스의 모든 지속 영웅의 체력에 효과 값을 추가하여 최대 체력으로 제한합니다.
UCLASS(ClassGroup=("Cos|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_Story_ModHeroHealth : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	UCardEffect_Story_ModHeroHealth();

protected:
	virtual void BeginPlay() override;

	virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
