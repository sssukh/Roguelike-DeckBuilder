// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/TargetSystem/TargetingComponent.h"
#include "TargetingComponent_SpecifiedTarget.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWNOFSIN_API UTargetingComponent_SpecifiedTarget : public UTargetingComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTargetingComponent_SpecifiedTarget();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
