// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEffectSystem/EffectState.h"

#include "Utilities/CosGameplayTags.h"


// Sets default values for this component's properties
UEffectState::UEffectState()
{
	
}

void UEffectState::ResetState()
{
	EffectLoopIndex = -1;
	TargetLoopIndex = -1;
	CurrentValidTargets.Empty();
	bInterrupt = false;
}

void UEffectState::ResetRepetition()
{
	Repetition = -1;
}




