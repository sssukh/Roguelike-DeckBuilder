// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent_AllOpposing.h"

#include "Algo/ForEach.h"
#include "Core/MinionBase.h"


// Sets default values for this component's properties
UTargetingComponent_AllOpposing::UTargetingComponent_AllOpposing()
{
	
}




bool UTargetingComponent_AllOpposing::FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect,
                                                       ACardBase* Card, bool bPreview, TArray<AActor*>& ValidTargets)
{
	ValidTargets.Reset();
	
	TArray<AMinionBase*> Minions;
	
	if(GetAllMinionsOnOpposingTrack(Card,Minions))
	{
		for (AActor* Minion : Minions)
		{
			ValidTargets.Add(Minion);
		}
		return true;
	}
	
	return false;
}

