#include "CombatSystem/AttackPatternComponent_SpiderQueen.h"

#include "Core/MinionBase.h"
#include "Core/MinionTrack.h"


UAttackPatternComponent_SpiderQueen::UAttackPatternComponent_SpiderQueen()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UAttackPatternComponent_SpiderQueen::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

ACardBase* UAttackPatternComponent_SpiderQueen::GetNextCard()
{
	AMinionBase* MinionOwner = Cast<AMinionBase>(GetOwner());
	if (!MinionOwner) return nullptr;

	if (MinionOwner->MinionTrack->Minions.Num() == 1 && PatternIndex > 1)
	{
		PatternIndex = -1;
	}

	return Super::GetNextCard();
}
