#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_ActiveDiscard.generated.h"

/*Flag.Active 태그가 있는 대상 액터를 삭제합니다. 필수 입력: 대상 배우(카드)*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_ActiveDiscard : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	UCardEffect_ActiveDiscard();

protected:
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;

public:
	UPROPERTY(BlueprintReadWrite, Category="Card Effect Active Discard")
	FGameplayTagContainer RelevantCardEventTags;
};
