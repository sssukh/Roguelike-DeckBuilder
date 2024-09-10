
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_AddNewCardToPile.generated.h"

/*카드 참조를 사용하거나, 사용할 수 없는 경우 데이터 테이블의 카드에 대한 참조를 사용하고 지정된 더미에 추가하려고 시도합니다.
 *필수 입력: TargetComponent(추가할 더미), 기존 카드의 Target(카드) 또는 새 카드를 추가할 UsedData(카드 데이터 테이블 행)*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_AddNewCardToPile : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCardEffect_AddNewCardToPile();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;

};
