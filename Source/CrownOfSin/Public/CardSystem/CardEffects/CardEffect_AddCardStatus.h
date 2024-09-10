
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_AddCardStatus.generated.h"

/*지정된 상태(TargetComponent에 설정됨)를 대상 카드에 추가하려고 시도합니다.
 *필수 입력: TargetComponent(상태), 효과 값(상태 값). 선택적 입력: 게임플레이 태그(상태에 추가됨)*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_AddCardStatus : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	UCardEffect_AddCardStatus();

protected:
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
