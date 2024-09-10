
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_ChangeNodeMap.generated.h"

/*게임 인스턴스의 현재 노드 맵을 식별자에 지정된 노드 맵으로 변경합니다.
 *이것이 다시 변경될 때까지 이 맵은 플레이어가 전투를 마친 후 돌아오는 맵이 됩니다. 데모에서 보스를 처치한 후 새로운 노드 맵으로 전환하는 데 사용됩니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_ChangeNodeMap : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCardEffect_ChangeNodeMap();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
