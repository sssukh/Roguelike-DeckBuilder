
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_ShowRewardScreen.generated.h"

/*LayoutWidget에 카드 보상 화면을 표시하는 작업을 대기열에 넣습니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_ShowRewardScreen : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	UCardEffect_ShowRewardScreen();

protected:
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
