
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_RevealShop.generated.h"


UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_RevealShop : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	UCardEffect_RevealShop();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
