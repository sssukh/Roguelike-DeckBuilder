// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetingComponent.h"
#include "TargetingComponent_Self.generated.h"

// 카드의 소유자를 대상으로 삼습니다.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UTargetingComponent_Self : public UTargetingComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTargetingComponent_Self();

	virtual bool FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect, AEffectActor* Card, bool bPreview, TArray<AActor*>& OutValidTargets) override;
};
