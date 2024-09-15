// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent_OpposingFront.h"
#include "Core/MinionBase.h"


// Sets default values for this component's properties
UTargetingComponent_OpposingFront::UTargetingComponent_OpposingFront()
{
	
}

bool UTargetingComponent_OpposingFront::FindValidTargets(TArray<AActor*>& SpecifiedTargets,
                                                         const FCardEffect& CardEffect, ACardBase* Card, bool bPreview, TArray<AActor*>& OutValidTargets)
{
	OutValidTargets.Reset();
	
	AMinionTrack* OwnerTrack = nullptr;
	AMinionTrack* OpposingTrack = nullptr;
	
	FindMinionTracks(Card,OwnerTrack,OpposingTrack);

	if(OpposingTrack && OpposingTrack->Minions.Num()>0)
	{
		OutValidTargets.Add(OpposingTrack->Minions[0]);
		return true;
	}

	return false;
}



