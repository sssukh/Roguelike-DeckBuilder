// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent_PayloadWithIdentifier.h"

#include "CardSystem/CardPlayer.h"
#include "Core/PayloadHolderComponent.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"


// Sets default values for this component's properties
UTargetingComponent_PayloadWithIdentifier::UTargetingComponent_PayloadWithIdentifier()
{
	
}

bool UTargetingComponent_PayloadWithIdentifier::FindValidTargets(TArray<AActor*>& SpecifiedTargets,
                                                                 const FCardEffect& CardEffect, ACardBase* Card, bool bPreview, TArray<AActor*>& OutValidTargets)
{
	OutValidTargets.Reset();
	
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	UObject* Payload = nullptr;

	if(CardPlayer->PayloadHolderComponent->AccessPayload(CardEffect.Identifier, !CardEffect.GameplayTags.HasTagExact(CosGameTags::Flag_DoNotClear),Payload))
	{
		if(AActor* Target = Cast<AActor>(Payload))
		{
			OutValidTargets.Add(Target);

			return true;
		}
	}

	return false;
}



