// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent_RandomOpposing.h"

#include "Core/MinionBase.h"
#include "Libraries/FunctionLibrary_ArrayUtils.h"


// Sets default values for this component's properties
UTargetingComponent_RandomOpposing::UTargetingComponent_RandomOpposing()
{
	
}

bool UTargetingComponent_RandomOpposing::FindValidTargets(TArray<AActor*>& SpecifiedTargets,
                                                          const FCardEffect& CardEffect, AEffectActor* Card, bool bPreview, TArray<AActor*>& OutValidTargets)
{
	OutValidTargets.Reset();
	
	TArray<AMinionBase*> Minions;
	if(!GetAllMinionsOnOpposingTrack(Card,Minions))
	{
		return false;
	}

	if(bPreview)
	{
		for (AMinionBase* Minion : Minions)
		{
			OutValidTargets.Add(Minion);
		}
	}
	else
	{
		int32 index = 0;
		UFunctionLibrary_ArrayUtils::GetRandomElementFromArray(Minions,index);
		OutValidTargets.Add(Minions[index]);
	}

	return true;
}




