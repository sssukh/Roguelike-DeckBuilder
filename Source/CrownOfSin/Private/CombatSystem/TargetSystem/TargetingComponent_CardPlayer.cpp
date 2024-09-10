// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent_CardPlayer.h"

#include "Libraries/FunctionLibrary_Singletons.h"

#include "CardSystem/CardPlayer.h"



// Sets default values for this component's properties
UTargetingComponent_CardPlayer::UTargetingComponent_CardPlayer()
{
	
}

bool UTargetingComponent_CardPlayer::FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect,
                                                      ACardBase* Card, bool bPreview, TArray<AActor*>& ValidTargets)
{
	ValidTargets.Reset();

	ACardPlayer* CardPlayer =UFunctionLibrary_Singletons::GetCardPlayer(this);
	
	ValidTargets.Add(CardPlayer);

	return true;
}




