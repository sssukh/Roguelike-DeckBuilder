// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetingComponent.h"
#include "TargetingComponent_SpecifiedCardsInHand.generated.h"


class UUW_CardHand;

/*
 *	손에 있는 카드들 중 지정된 GameplayTag를 가진 카드들을 대상으로 합니다.
 *	태그가 지정되있지 않으면 손에 있는 모든 카드를 대상으로 합니다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UTargetingComponent_SpecifiedCardsInHand : public UTargetingComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTargetingComponent_SpecifiedCardsInHand();

	virtual bool FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect, ACardBase* Card, bool bPreview, TArray<AActor*>& OutValidTargets) override;

	UFUNCTION(BlueprintCallable, Category = "TargetSystem SpecifiedCardsInHand")
	void BindToCardConfirm(UUW_CardHand* Hand);

	UFUNCTION(BlueprintCallable, Category = "TargetSystem SpecifiedCardsInHand")
	void ValidateAndTransferSelectedCards(const  TArray<ACardBase*>& Cards);

	// Member Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetSystem SpecifiedCardsInHand")
	FGameplayTagContainer EmptyTagContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetSystem SpecifiedCardsInHand")
	TObjectPtr<UUW_CardHand> BoundHand;
};
