
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_AddRandomArtifacts.generated.h"

/*효과 값에 따라 무작위 아티팩트를 여러 개 가져와 대상에 추가합니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_AddRandomArtifacts : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	UCardEffect_AddRandomArtifacts();

protected:
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
