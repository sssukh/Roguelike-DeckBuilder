﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem//TargetingComponent_SpecifiedTarget.h"


// Sets default values for this component's properties
UTargetingComponent_SpecifiedTarget::UTargetingComponent_SpecifiedTarget()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTargetingComponent_SpecifiedTarget::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTargetingComponent_SpecifiedTarget::TickComponent(float DeltaTime, ELevelTick TickType,
                                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

