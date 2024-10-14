// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetingComponent.h"
#include "TargetingComponent_AllOpposing.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UTargetingComponent_AllOpposing : public UTargetingComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTargetingComponent_AllOpposing();

	// 카드를 소유한 미니언이 있는 트랙의 반대편에 있는 모든 미니언을 대상으로 삼는다.
	virtual bool FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect, AEffectActor* Card, bool bPreview, TArray<AActor*>& OutValidTargets) override;
};
