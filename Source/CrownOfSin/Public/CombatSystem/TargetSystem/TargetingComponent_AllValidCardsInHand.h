// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetingComponent.h"
#include "TargetingComponent_AllValidCardsInHand.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UTargetingComponent_AllValidCardsInHand : public UTargetingComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTargetingComponent_AllValidCardsInHand();

	// 플레이어의 손에 있는 카드들 중에서 지정된 태그 중 하나라도 소유한 카드들을 대상으로 삼는다.
	// 태그가 지정되지 않은 경우 손에 있는 모든 카드를 대상으로 한다.
	virtual bool FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect, ACardBase* Card, bool bPreview, TArray<AActor*>& OutValidTargets) override;

};
