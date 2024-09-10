
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_AddToStatusCapped.generated.h"

/*AddToStatus와 유사하지만 최대 결과 상태 값을 대상 상태의 최대 값 변수로 고정합니다. 사용 예: 체력이 최대치 이상으로 증가하면 치유를 방지합니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_AddToStatusCapped : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCardEffect_AddToStatusCapped();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
