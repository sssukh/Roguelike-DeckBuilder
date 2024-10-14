// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetingComponent.h"
#include "TargetingComponent_Untargeted.generated.h"

// 타겟이 필요하지 않은 카드 이펙트에 사용됩니다.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UTargetingComponent_Untargeted : public UTargetingComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTargetingComponent_Untargeted();

	virtual bool FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect, AEffectActor* Card, bool bPreview, TArray<AActor*>& OutValidTargets) override;
};
