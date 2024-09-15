#include "CardSystem/CardEffects/Story/CardEffect_Story_Encounter.h"

#include "CardSystem/CardPlayer.h"
#include "Libraries/FunctionLibrary_Singletons.h"


UCardEffect_Story_Encounter::UCardEffect_Story_Encounter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}

void UCardEffect_Story_Encounter::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_Story_Encounter::ResolveCardEffect(AActor* TargetActor)
{
	if (ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this))
	{
		IInterface_StoryEncounter::Execute_InitializeStoryEncounter(CardPlayer, UsedData, false);
	}
	return true;
}
