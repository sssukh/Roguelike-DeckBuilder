#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_RemoveCardFromPile.generated.h"


UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_RemoveCardFromPile : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	UCardEffect_RemoveCardFromPile();

protected:
	virtual void BeginPlay() override;

public:

	virtual bool ResolveCardEffect(AActor* TargetActor) override;
	
};
