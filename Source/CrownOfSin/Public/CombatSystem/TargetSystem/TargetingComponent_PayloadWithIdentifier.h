// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetingComponent.h"
#include "TargetingComponent_PayloadWithIdentifier.generated.h"

// 이전 StoredTargetAsPayload 카드 효과에서 페이로드로 저장된 객체를 대상으로 삼습니다.
// 이 타게팅 모드를 가진 카드 이펙트의 Identifier는 반드시 이전 카드 이펙트의 Identifier와 일치해야합니다.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UTargetingComponent_PayloadWithIdentifier : public UTargetingComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTargetingComponent_PayloadWithIdentifier();

	virtual bool FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect, ACardBase* Card, bool bPreview, TArray<AActor*>& OutValidTargets) override;
};
