
#pragma once

#include "CoreMinimal.h"
#include "CardSystem/CardEffects/CardEffectComponent.h"
#include "CardEffect_Story_NokkenAlter.generated.h"

/*공격 또는 방어인 경우 대상 카드의 효과 값을 두 배로 늘리고, 다른 종류의 카드인 경우 비용을 0으로 설정합니다.
 *또한 카드의 색조를 녹색으로 변경하고 이름 뒤에 "+"를 추가합니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_Story_NokkenAlter : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCardEffect_Story_NokkenAlter();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
	
};
