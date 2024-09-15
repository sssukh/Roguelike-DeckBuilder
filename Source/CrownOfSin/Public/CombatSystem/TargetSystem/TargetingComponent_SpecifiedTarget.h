// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/TargetSystem/TargetingComponent.h"
#include "TargetingComponent_SpecifiedTarget.generated.h"

/*
 *	 이 타게팅모드에 입력된 모든 지정된 타겟들을 대상으로 삼습니다.
 *	 일반적으로 플레이어가 직접 대상으로 삼은 미니언들을 타게팅 모드에 보내는데 사용됩니다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UTargetingComponent_SpecifiedTarget : public UTargetingComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTargetingComponent_SpecifiedTarget();

	virtual bool FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect, ACardBase* Card, bool bPreview, TArray<AActor*>& OutValidTargets) override;
};
