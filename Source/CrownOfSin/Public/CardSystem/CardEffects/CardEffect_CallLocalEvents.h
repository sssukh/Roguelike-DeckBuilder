
#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_CallLocalEvents.generated.h"


/*대상의 로컬 Dispatcher Hub에서 카드 효과에 지정된 이벤트를 호출합니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_CallLocalEvents : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	UCardEffect_CallLocalEvents();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
