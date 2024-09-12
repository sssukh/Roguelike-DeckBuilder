
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_UseCard.generated.h"

/*대상 없이 대상 카드를 사용하려고 시도합니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_UseCard : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCardEffect_UseCard();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
