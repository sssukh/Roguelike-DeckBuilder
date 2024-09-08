
#pragma once

#include "CoreMinimal.h"
#include "CardSystem/CardEffects/CardEffectComponent.h"
#include "CardEffect_Story_DisableButtons.generated.h"

/*플레이어가 이런 식으로 진행하지 못하도록 스토리를 진행하는 동안 활성 스토리 버튼을 일시적으로 비활성화하는 데 사용됩니다.*/
UCLASS(ClassGroup=("COS|Card"), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_Story_DisableButtons : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCardEffect_Story_DisableButtons();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual bool ResolveCardEffect(AActor* TargetActor) override;
};
