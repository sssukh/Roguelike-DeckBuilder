// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CardEffectComponent.h"
#include "CardEffect_AddRandomCardsToPile.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UCardEffect_AddRandomCardsToPile : public UCardEffectComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCardEffect_AddRandomCardsToPile();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
