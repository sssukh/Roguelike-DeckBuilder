#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "GameplayTagContainer.h"
#include "Action_AdvAnim.generated.h"

/*퍼펫에서 애니메이션을 호출하고 다음 액션으로 진행하기 전에 퍼펫이 애니메이션이 끝났다는 신호를 보낼 때까지 기다리는 액션*/
UCLASS()
class CROWNOFSIN_API AAction_AdvAnim : public AActionBase
{
	GENERATED_BODY()

public:
	AAction_AdvAnim();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PlayAction_Implementation() override;

protected:
	UFUNCTION()
	void CheckAnimationEvent(AActor* InPuppet, FGameplayTag InAnimationTag);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Adv Anim", meta=(ExposeOnSpawn="true"))
	FGameplayTag Animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Adv Anim", meta=(ExposeOnSpawn="true"))
	AActor* Puppet;
};
