﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackPatternComponent.h"
#include "AttackPatternComponent_SpiderQueen.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UAttackPatternComponent_SpiderQueen : public UAttackPatternComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttackPatternComponent_SpiderQueen();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
