
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_OpenNodeMap.generated.h"

/*현재 게임 인스턴스에 설정된 노드 맵 레벨을 엽니다.
 *또는 카드 효과 식별자에 레벨 이름이 지정된 경우 대신 해당 레벨을 열고 인스턴스의 현재 노드 맵을 이 새 노드 맵으로 변경합니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_OpenNodeMap : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCardEffect_OpenNodeMap();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
