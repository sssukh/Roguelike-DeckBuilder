// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetingComponent.h"
#include "TargetingComponent_SpecifiedCardsInHand.generated.h"


class UUW_CardHand;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UTargetingComponent_SpecifiedCardsInHand : public UTargetingComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTargetingComponent_SpecifiedCardsInHand();

	virtual bool FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect, ACardBase* Card, bool bPreview, TArray<AActor*>& ValidTargets) override;

	UFUNCTION(BlueprintCallable, Category = "TargetSystem SpecifiedCardsInHand")
	void BindToCardConfirm(UUW_CardHand* Hand);

	UFUNCTION(BlueprintCallable, Category = "TargetSystem SpecifiedCardsInHand")
	void ValidateAndTransferSelectedCards(TArray<ACardBase*>& Cards);

	// Member Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetSystem SpecifiedCardsInHand")
	FGameplayTagContainer EmptyTagContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetSystem SpecifiedCardsInHand")
	TObjectPtr<UUW_CardHand> BoundHand;
};
