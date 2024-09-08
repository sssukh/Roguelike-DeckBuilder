
#pragma once

#include "CoreMinimal.h"
#include "CardSystem/CardEffects/CardEffectComponent.h"
#include "CardEffect_Story_Encounter.generated.h"


UCLASS(ClassGroup=("Cos|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_Story_Encounter : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	
	UCardEffect_Story_Encounter();

protected:
	
	virtual void BeginPlay() override;

public:

	virtual bool ResolveCardEffect(AActor* TargetActor) override;
	
};
