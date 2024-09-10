#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_DrawSpecificCard.generated.h"

/*이 카드 효과의 타겟팅 모드에 의해 지정된 카드를 카드 플레이어의 손에 가져옵니다(카드가 뽑기 더미에 있는 경우).*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_DrawSpecificCard : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCardEffect_DrawSpecificCard();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
