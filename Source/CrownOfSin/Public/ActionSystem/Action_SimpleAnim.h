// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionBase.h"
#include "GameplayTagContainer.h"
#include "Action_SimpleAnim.generated.h"

UCLASS()
class CROWNOFSIN_API AAction_SimpleAnim : public AActionBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAction_SimpleAnim();

	virtual void PlayAction_Implementation() override;


	// Member Variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action SimpleAnim")
	TObjectPtr<AActor> Puppet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Action SimpleAnim")
	FGameplayTag Animation;
};
