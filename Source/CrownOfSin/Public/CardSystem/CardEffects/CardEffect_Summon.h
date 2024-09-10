
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_Summon.generated.h"


/*카드 소유자의 미니언 트랙에 효과 값과 같은 수의 미니언을 추가합니다.
 *미니언 유형은 카드 효과의 UsedDataTable에 정의되어 있습니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_Summon : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCardEffect_Summon();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual  bool ResolveCardEffect(AActor* TargetActor) override;
};
