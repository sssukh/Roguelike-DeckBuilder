
#pragma once

#include "CoreMinimal.h"
#include "CardSystem/CardEffects/CardEffectComponent.h"
#include "CardEffect_Story_ArenaEncounter.generated.h"

/*사용된 데이터에 저장된 모든 인카운터 데이터에 액세스하여 게임 인스턴스에서 이 인카운터를 설정하고 인카운터에 지정된 레벨을 엽니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_Story_ArenaEncounter : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCardEffect_Story_ArenaEncounter();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
