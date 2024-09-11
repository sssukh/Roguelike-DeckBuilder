// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetingComponent.h"
#include "TargetingComponent_RandomCardsInHand.generated.h"

/*
 *	손에 있는 카드들 중 지정된 GameplayTag를 가진 랜덤한 카드들을 effect Value 값만큼 대상으로 삼습니다.
 *	GameplayTag가 지정되지 않으면 무작위로 선정됩니다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UTargetingComponent_RandomCardsInHand : public UTargetingComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTargetingComponent_RandomCardsInHand();

	virtual bool FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect, ACardBase* Card, bool bPreview, TArray<AActor*>& ValidTargets) override;
};
