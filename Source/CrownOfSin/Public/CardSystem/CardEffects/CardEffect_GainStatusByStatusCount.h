
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_GainStatusByStatusCount.generated.h"

/*지정된 게임플레이 태그가 있는 대상 액터의 모든 현재 상태의 합산 값과 동일한 상태 값을 사용하여 대상 구성 요소에 지정된 상태를 대상에 추가합니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_GainStatusByStatusCount : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCardEffect_GainStatusByStatusCount();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
