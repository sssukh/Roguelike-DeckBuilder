#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "GameplayTagContainer.h"
#include "Core/CosEnumStruct.h"
#include "Action_Effect.generated.h"

UCLASS()
class CROWNOFSIN_API AAction_Effect : public AActionBase
{
	GENERATED_BODY()

public:
	AAction_Effect();

protected:
	virtual void BeginPlay() override;

public:
	/**
	 * @brief 주어진 애니메이션 또는 지연 시간 후에 효과 파티클을 스폰하는 액션을 실행합니다.
	 * 
	 * 애니메이션이 있으면 애니메이션 이벤트가 발생할 때까지 대기하고, 애니메이션이 없으면 즉시 또는 지연 후 파티클을 스폰합니다.
	 */	
	virtual void PlayAction_Implementation() override;

	/**
	 * @brief 타겟 위치에 이펙트 파티클을 스폰하고, 애니메이션 이벤트 발생 시 해당 이벤트를 처리합니다.
	 * 
	 * 타겟과 파티클이 유효한 경우, 카메라와 타겟 사이에서 보간된 위치에 파티클을 스폰합니다. 
	 * 또한 애니메이션 이벤트 발생 시 애니메이션에 맞춰 파티클을 스폰합니다.
	 */	
	void SpawnEffectParticles();
	
protected:

	/**
	 * @brief 애니메이션 이벤트가 발생할 때 해당 애니메이션에 맞춰 파티클을 스폰합니다.
	 * 
	 * @param InPuppet 애니메이션 이벤트가 발생한 퍼핏 액터
	 * @param InAnimationTag 발생한 애니메이션 이벤트의 태그
	 */	
	UFUNCTION()
	void CheckAnimationEvent(AActor* InPuppet, FGameplayTag InAnimationTag);


	/*========================================================================================
	*	Field Members
	=========================================================================================*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Effect")
	AActor* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Effect")
	AActor* SourcePuppet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Effect")
	FGameplayTagContainer AnimTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Effect")
	FGameplayTag HeroAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Effect")
	FCardEffect Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Effect")
	float StartDelayIfNoAnim;
};
