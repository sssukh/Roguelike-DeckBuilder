
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_AddSpecifiedArtifact.generated.h"

/*Target Component에 지정된 Artifact Status를 Target에 추가합니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_AddSpecifiedArtifact : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	UCardEffect_AddSpecifiedArtifact();

protected:
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
